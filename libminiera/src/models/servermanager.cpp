#include "models/servermanager.h"
#include <fstream>
#include <libnick/filesystem/userdirectories.h>

using namespace Nickvision::Filesystem;

namespace Nickvision::Miniera::Shared::Models
{
    ServerManager::ServerManager(const std::string& appName)
        : m_serversDirectory{ UserDirectories::get(ApplicationUserDirectory::LocalData, appName) / "Servers" }
    {
        std::filesystem::create_directories(m_serversDirectory);
    }

    const std::vector<std::shared_ptr<Server>>& ServerManager::getAvailableServers()
    {
        if(m_servers.empty())
        {
            for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_serversDirectory))
            {
                std::filesystem::path jsonPath{ entry.path() / "miniera.json" };
                if(entry.is_directory() && std::filesystem::exists(jsonPath))
                {
                    std::ifstream file{ jsonPath };
                    boost::json::stream_parser parser;
                    std::string line;
                    while(std::getline(file, line))
                    {
                        parser.write(line);
                    }
                    parser.finish();
                    boost::json::value value = parser.release();
                    if(value.is_object())
                    {
                        m_servers.push_back(std::make_shared<Server>(value.get_object()));
                    }
                }
            }
        }
        return m_servers;
    }

    bool ServerManager::getServerExists(const std::string& name) const
    {
        return std::filesystem::exists(m_serversDirectory / name);
    }

    std::shared_ptr<Server> ServerManager::createServer(const ServerVersion& version, const ServerProperties& properties)
    {
        if(getServerExists(properties.getLevelName()))
        {
            return nullptr;
        }
        m_servers.push_back(std::make_shared<Server>(version, properties, m_serversDirectory / properties.getLevelName()));
        return m_servers.back();
    } 
}