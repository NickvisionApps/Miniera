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
        //Load Servers
        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_serversDirectory))
        {
            std::string name{ entry.path().filename().string() };
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
                if(value.is_object() )
                {
                    m_servers.emplace(name, std::make_shared<Server>(value.get_object()));
                }
            }
        }
    }

    std::vector<std::string> ServerManager::getAvailableServersNames()
    {
        std::vector<std::string> names;
        for(const std::pair<const std::string, std::shared_ptr<Server>>& pair : m_servers)
        {
            names.push_back(pair.first);
        }
        return names;
    }

    bool ServerManager::getServerExists(const std::string& name) const
    {
        return m_servers.contains(name);
    }

    const std::shared_ptr<Server>& ServerManager::createServer(const ServerVersion& version, const ServerProperties& properties)
    {
        if(getServerExists(properties.getLevelName()))
        {
            static std::shared_ptr<Server> null{ nullptr };
            return null;
        }
        m_servers.emplace(properties.getLevelName(), std::make_shared<Server>(version, properties, m_serversDirectory / properties.getLevelName()));
        return m_servers.at(properties.getLevelName());
    }
    
    const std::shared_ptr<Server>& ServerManager::getServer(const std::string& name)
    {
        if(!m_servers.contains(name))
        {
            static std::shared_ptr<Server> null{ nullptr };
            return null;
        }
        return m_servers.at(name);
    }
}
