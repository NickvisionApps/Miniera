#include "controllers/serverviewcontroller.h"
#include <chrono>
#include <stdexcept>
#include <thread>

using namespace Nickvision::Events;
using namespace Nickvision::Miniera::Shared::Models;

namespace Nickvision::Miniera::Shared::Controllers
{
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

    Event<ParamEventArgs<std::string>>& ServerViewController::consoleOutputChanged()
    {
        return m_consoleOutputChanged;
    }

    const std::string& ServerViewController::getName() const
    {
        return m_server->getName();
    }

    std::string ServerViewController::getVersion() const
    {
        return m_server->getVersion().str();
    }

    bool ServerViewController::supportsMods() const
    {
        return m_server->getVersion().getEdition() == Edition::Forge;
    }

    PowerStatus ServerViewController::startStop()
    {
        if(m_server->isRunning())
        {
            if(m_server->stop())
            {
                if(m_watcher.joinable())
                {
                    m_watcher.join();
                }
                return PowerStatus::Stopped;
            }
            return PowerStatus::ErrorStopping;
        }
        if(m_server->start(m_configuration.getMaxServerRamInGB()))
        {
            m_watcher = std::thread(&ServerViewController::watch, this);
            return PowerStatus::Started;
        }
        return PowerStatus::ErrorStarting;
    }

    void ServerViewController::sendCommand(const std::string& cmd)
    {
        m_server->command(cmd);
    }

    void ServerViewController::watch()
    {
        size_t consoleOutputSize{ 0 };
        while(m_server->isRunning())
        {
            //Check console output
            if(consoleOutputSize != m_server->getOutput().size())
            {
                consoleOutputSize = m_server->getOutput().size();
                m_consoleOutputChanged.invoke({ m_server->getOutput() });
            }
            //Sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}
