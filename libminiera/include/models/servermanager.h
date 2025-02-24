#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <filesystem>
#include <memory>
#include <string>
#include <vector>
#include "server.h"
#include "serverproperties.h"
#include "serverversion.h" 

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A manager of servers.
     */
    class ServerManager
    {
    public:
        /**
         * @brief Constructs a ServerManager.
         * @param appName The name of the app
         */
        ServerManager(const std::string& appName);
        /**
         * @brief Gets the available servers.
         * @return The list of available servers
         */
        const std::vector<std::shared_ptr<Server>>& getAvailableServers();
        /**
         * @brief Gets whether or not a server with the given name exists.
         * @param name The name of the server
         * @return True if the server exists
         */
        bool getServerExists(const std::string& name) const;
        /**
         * @brief Creates a server.
         * @param version The version of the server
         * @param properties The properties of the server
         * @return The created server
         */
        std::shared_ptr<Server> createServer(const ServerVersion& version, const ServerProperties& properties);

    private:
        std::filesystem::path m_serversDirectory;
        std::vector<std::shared_ptr<Server>> m_servers;
    };
}

#endif //SERVERMANAGER_H