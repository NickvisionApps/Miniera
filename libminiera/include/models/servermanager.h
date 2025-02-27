#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
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
         * @brief Gets the names of the available servers.
         * @return The list of available server names
         */
        std::vector<std::string> getAvailableServersNames();
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
        const std::shared_ptr<Server>& createServer(const ServerVersion& version, const ServerProperties& properties);
        /**
         * @brief Gets a server.
         * @param name The name of the server to get
         * @return The server if found, else nullptr
         */
        const std::shared_ptr<Server>& getServer(const std::string& name);

    private:
        std::filesystem::path m_serversDirectory;
        std::unordered_map<std::string, std::shared_ptr<Server>> m_servers;
    };
}

#endif //SERVERMANAGER_H