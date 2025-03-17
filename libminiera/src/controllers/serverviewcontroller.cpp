#include "controllers/serverviewcontroller.h"
#include <chrono>
#include <format>
#include <stdexcept>
#include <thread>
#include <libnick/helpers/codehelpers.h>
#include <libnick/helpers/sizehelpers.h>
#include <libnick/localization/gettext.h>

using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Miniera::Shared::Models;

namespace Nickvision::Miniera::Shared::Controllers
{
    template<SizeHelpers::Numeric T>
    static int getDigitCount(T value)
    {
        int count = 0;
        while(value > 0)
        {
            count++;
            value /= 10;
        }
        return count;
    }

    ServerViewController::ServerViewController(const std::shared_ptr<Server>& server, const Configuration& configuration)
        : m_server{ server },
        m_configuration{ configuration }
    {
        if(!server)
        {
            throw std::invalid_argument("Server cannot be null");
        }
    }

    ServerViewController::~ServerViewController()
    {
        m_server->stop();
        if(m_watcher.joinable())
        {
            m_watcher.join();
        }
    }

    Event<ParamEventArgs<PowerStatus>>& ServerViewController::powerChanged()
    {
        return m_powerChanged;
    }

    Event<ParamEventArgs<ServerAddress>>& ServerViewController::addressChanged()
    {
        return m_addressChanged;
    }

    Event<ParamEventArgs<std::string>>& ServerViewController::consoleOutputChanged()
    {
        return m_consoleOutputChanged;
    }

    Event<ParamEventArgs<std::pair<double, unsigned long long>>>& ServerViewController::resourceUsageChanged()
    {
        return m_resourceUsageChanged;
    }

    const std::string& ServerViewController::getName() const
    {
        return m_server->getName();
    }

    std::string ServerViewController::getVersion() const
    {
        return m_server->getVersion().str();
    }

    ServerAddress ServerViewController::getAddress() const
    {
        return m_server->getAddress();
    }

    bool ServerViewController::isRunning() const
    {
        return m_server->isRunning();
    }

    bool ServerViewController::supportsMods() const
    {
        return m_server->getVersion().getEdition() == Edition::Forge;
    }

    const std::vector<std::string>& ServerViewController::getMods() const
    {
        return m_server->getMods();
    }

    unsigned long ServerViewController::getMaxServerRamInMB() const
    {
        return static_cast<unsigned long>(SizeHelpers::bytesToMegabytes(SizeHelpers::gigabytesToBytes<unsigned long long>(m_configuration.getMaxServerRamInGB())));
    }

    std::string ServerViewController::getRAMString(unsigned long long bytes) const
    {
        unsigned long long converted{ bytes };
        std::string res{ std::vformat(_("{} B / {} GB"), std::make_format_args(converted, CodeHelpers::unmove(m_configuration.getMaxServerRamInGB()))) };
        for(int i = 0; getDigitCount(converted) > 4; i++)
        {
            if(i == 0)
            {
                converted = SizeHelpers::bytesToKilobytes(bytes);
                res = std::vformat(_("{} KB / {} GB"), std::make_format_args(converted, CodeHelpers::unmove(m_configuration.getMaxServerRamInGB())));
            }
            else if(i == 1)
            {
                converted = SizeHelpers::bytesToMegabytes(bytes);
                res = std::vformat(_("{} MB / {} GB"), std::make_format_args(converted, CodeHelpers::unmove(m_configuration.getMaxServerRamInGB())));
            }
            else if(i == 2)
            {
                converted = SizeHelpers::bytesToGigabytes(bytes);
                return std::vformat(_("{} GB / {} GB"), std::make_format_args(converted, CodeHelpers::unmove(m_configuration.getMaxServerRamInGB())));
            }
        }
        return res;
    }

    void ServerViewController::startStop()
    {
        std::thread worker{ [this]()
        {
            if(m_server->isRunning())
            {
                if(m_server->stop())
                {
                    if(m_watcher.joinable())
                    {
                        m_watcher.join();
                    }
                }
                else
                {
                    m_powerChanged.invoke({ PowerStatus::ErrorStopping });
                }
            }
            else if(m_server->start(m_configuration.getMaxServerRamInGB()))
            {
                if(m_watcher.joinable())
                {
                    m_watcher.join();
                }
                m_watcher = std::thread(&ServerViewController::watch, this);
            }
            else
            {
                m_powerChanged.invoke({ PowerStatus::ErrorStarting });
            }
        } };
        worker.detach();
    }

    void ServerViewController::broadcast()
    {
        if(m_configuration.getNgrokAuthToken().empty())
        {
            m_addressChanged.invoke({{}});
        }
        std::thread worker{ [this]()
        {
            m_addressChanged.invoke({ m_server->broadcast(m_configuration.getNgrokAuthToken()) });
        } };
        worker.detach();
    }

    void ServerViewController::sendCommand(const std::string& cmd)
    {
        m_server->command(cmd);
    }

    bool ServerViewController::uploadMod(const std::filesystem::path& file)
    {
        return m_server->uploadMod(file, m_configuration.getDeleteModAfterUpload());
    }

    bool ServerViewController::removeMod(const std::string& mod)
    {
        return m_server->removeMod(mod);
    }

    void ServerViewController::watch()
    {
        size_t consoleOutputSize{ 0 };
        double oldCPU{ 0 };
        unsigned long long oldRAM{ 0 };
        m_powerChanged.invoke({ PowerStatus::Started });
        m_addressChanged.invoke({ m_server->getAddress() });
        while(m_server->isRunning())
        {
            //Console output
            if(consoleOutputSize != m_server->getOutput().size())
            {
                consoleOutputSize = m_server->getOutput().size();
                m_consoleOutputChanged.invoke({ m_server->getOutput() });
            }
            //Resoures
            if(oldCPU != m_server->getCPUUsage() || oldRAM != m_server->getRAMUsage())
            {
                oldCPU = m_server->getCPUUsage();
                oldRAM = m_server->getRAMUsage();
                m_resourceUsageChanged.invoke({ std::make_pair(oldCPU, oldRAM) });
            }
            //Sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        m_powerChanged.invoke({ PowerStatus::Stopped });
        m_addressChanged.invoke({ m_server->getAddress() });
        m_resourceUsageChanged.invoke({ std::make_pair(0.0, 0L) });
    }
}
