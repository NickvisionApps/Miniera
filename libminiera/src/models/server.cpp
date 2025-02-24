#include "models/server.h"
#include <fstream>

namespace Nickvision::Miniera::Shared::Models
{
    Server::Server(const ServerVersion& serverVersion, const ServerProperties& serverProperties, const std::filesystem::path& serverDirectory)
        : m_serverVersion{ serverVersion },
        m_serverProperties{ serverProperties },
        m_serverDirectory{ serverDirectory }
    {
        std::filesystem::create_directories(m_serverDirectory);
        writeJsonToDisk();
    }

    Server::Server(boost::json::object json)
        : m_serverVersion{ json["Version"].as_object() },
        m_serverProperties{ json["Properties"].as_object() },
        m_serverDirectory{ json["Directory"].as_string().c_str() }
    {

    }

    const std::string& Server::getName() const
    {
        return m_serverProperties.getLevelName();
    }

    boost::json::object Server::toJson() const
    {
        boost::json::object json;
        json["Version"] = m_serverVersion.toJson();
        json["Properties"] = m_serverProperties.toJson();
        json["Directory"] = m_serverDirectory.string();
        return json;
    }

    void Server::writeJsonToDisk() const
    {
        std::ofstream file{ m_serverDirectory / "miniera.json" };
        file << toJson() << std::endl;
    }
}