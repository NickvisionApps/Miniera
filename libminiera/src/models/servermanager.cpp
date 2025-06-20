#include "models/servermanager.h"
#include <fstream>
#include <libnick/filesystem/userdirectories.h>
#include "controllers/serverviewcontroller.h"

using namespace Nickvision::Events;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Miniera::Shared::Controllers;
using namespace Nickvision::Miniera::Shared::Events;

namespace Nickvision::Miniera::Shared::Models
{
    ServerManager::ServerManager(const std::string& appName, const Configuration& configuration)
        : m_configuration{ configuration },
        m_serversDirectory{ UserDirectories::get(ApplicationUserDirectory::LocalData, appName) / "Servers" }
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
                    m_loadedServers.emplace(name, false);
                }
            }
        }
    }

    Event<ServerInitializationProgressChangedEventArgs>& ServerManager::serverInitializationProgressChanged()
    {
        return m_serverInitializationProgressChanged;
    }

    Event<ServerLoadedEventArgs>& ServerManager::serverLoaded()
    {
        return m_serverLoaded;
    }

    Event<ParamEventArgs<std::string>>& ServerManager::serverDeleted()
    {
        return m_serverDeleted;
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

    bool ServerManager::createServer(const ServerVersion& version, const ServerProperties& properties)
    {
        if(getServerExists(properties.getLevelName()))
        {
            return false;
        }
        std::shared_ptr<Server> server{ std::make_shared<Server>(version, properties, m_serversDirectory / properties.getLevelName()) };
        m_servers.emplace(server->getName(), server);
        m_loadedServers.emplace(server->getName(), false);
        server->initializationProgressChanged() += [this, server](const ServerInitializationProgressChangedEventArgs& args)
        {
            m_serverInitializationProgressChanged.invoke(args);
            if(args.isFinished() && !args.isError())
            {
                m_loadedServers[server->getName()] = true;
                m_serverLoaded.invoke({ server->getName(), std::make_shared<ServerViewController>(server, m_configuration) });
            }
        };
        server->initialize();
        return true;
    }
    
    bool ServerManager::loadServer(const std::string& name)
    {
        if(!m_servers.contains(name) || m_loadedServers.at(name))
        {
            return false;
        }
        //Ensure server is initialized in case previously failed
        m_servers.at(name)->initializationProgressChanged() += [this, name](const ServerInitializationProgressChangedEventArgs& args)
        {
            m_serverInitializationProgressChanged.invoke(args);
            if(args.isFinished() && !args.isError())
            {
                m_loadedServers[name] = true;
                m_serverLoaded.invoke({ name, std::make_shared<ServerViewController>(m_servers.at(name), m_configuration) });
            }
        };
        m_servers.at(name)->initialize();
        return true;
    }

    bool ServerManager::deleteServer(const std::string& name)
    {
        if(!m_servers.contains(name) || m_servers.at(name)->isRunning())
        {
            return false;
        }
        m_servers.erase(name);
        m_loadedServers.erase(name);
        std::thread worker{ [this, name]()
        {
            std::filesystem::remove_all(m_serversDirectory / name);
            m_serverDeleted.invoke({ name });
        } };
        worker.detach();
        return true;
    }
}
