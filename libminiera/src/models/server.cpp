#include "models/server.h"
#include <fstream>
#include <stdexcept>
#include <libnick/network/web.h>

using namespace Nickvision::Network;

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

    bool Server::initialize()
    {
        if(!m_isInitialized)
        {
            //Download and extract server files
            //Add eula = true file
            //m_isInitialized = true;
            //writeJsonToDisk();
        }
        return m_isInitialized;
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

    int Server::onInitializationDownloadProgressChanged(curl_off_t dltotal, curl_off_t dlnow, curl_off_t utotal, curl_off_t unow)
    {
        return 0;
    }
}
