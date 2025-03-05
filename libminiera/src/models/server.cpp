#include "models/server.h"
#include <fstream>
#include <stdexcept>
#include <thread>
#include <libnick/localization/gettext.h>
#include "helpers/serverinitializationhelpers.h"

#define SERVER_INITIALIZATION_HELPER_ARGS log, m_serverDirectory, getName(), m_serverVersion, m_initializationProgressChanged

using namespace Nickvision::Events;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Miniera::Shared::Helpers;

namespace Nickvision::Miniera::Shared::Models
{
    Server::Server(const ServerVersion& serverVersion, const ServerProperties& serverProperties, const std::filesystem::path& serverDirectory)
        : m_serverVersion{ serverVersion },
        m_serverProperties{ serverProperties },
        m_serverDirectory{ serverDirectory },
        m_isInitialized{ false }
    {
        std::filesystem::create_directories(m_serverDirectory);
        writeJsonToDisk();
    }

    Server::Server(boost::json::object json)
        : m_serverVersion{ json["Version"].is_object() ? json["Version"].as_object() : boost::json::object() },
        m_serverProperties{ json["Properties"].is_object() ? json["Properties"].as_object() : boost::json::object() },
        m_serverDirectory{ json["Directory"].is_string() ? json["Directory"].as_string().c_str() : "" },
        m_isInitialized{ json["IsInitialized"].is_bool() ? json["IsInitialized"].as_bool() : false }
    {
        if(!std::filesystem::exists(m_serverDirectory))
        {
            throw std::invalid_argument("Invalid JSON object");
        }
    }

    Event<ServerInitializationProgressChangedEventArgs>& Server::initializationProgressChanged()
    {
        return m_initializationProgressChanged;
    }

    const std::string& Server::getName() const
    {
        return m_serverProperties.getLevelName();
    }

    const ServerVersion& Server::getVersion() const
    {
        return m_serverVersion;
    }

    bool Server::isInitialized() const
    {
        return m_isInitialized;
    }

    void Server::initialize()
    {
        if(m_isInitialized)
        {
            m_initializationProgressChanged.invoke({ getName(), 1.0, _("[Initialization] Server already initialized."), true, false });
            return;
        }
        std::thread worker{ [this]()
        {
            std::string log{ _("[Initialization] Starting server initialization process...") };
            if(!ServerInitializationHelpers::download(SERVER_INITIALIZATION_HELPER_ARGS))
            {
                return;
            }
            if(!ServerInitializationHelpers::extract(SERVER_INITIALIZATION_HELPER_ARGS))
            {
                return;
            }
            if(!ServerInitializationHelpers::write(SERVER_INITIALIZATION_HELPER_ARGS))
            {
                return;
            }
            m_isInitialized = true;
            writeJsonToDisk();
            log += _("[Initialization] Server initialization process completed..");
            m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, false });
        } };
        worker.detach();
    }

    boost::json::object Server::toJson() const
    {
        boost::json::object json;
        json["Version"] = m_serverVersion.toJson();
        json["Properties"] = m_serverProperties.toJson();
        json["Directory"] = m_serverDirectory.string();
        json["IsInitialized"] = m_isInitialized;
        return json;
    }

    void Server::writeJsonToDisk() const
    {
        std::ofstream file{ m_serverDirectory / "miniera.json" };
        file << toJson() << std::endl;
    }
}
