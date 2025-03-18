#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <libnick/events/event.h>
#include "events/serverinitializationprogresschangedeventargs.h"
#include "events/serverloadedeventargs.h"
#include "configuration.h"
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
         * @param configuration The app's configuration
         */
        ServerManager(const std::string& appName, const Configuration& configuration);
        /**
         * @brief Gets the event for when a server's initialization's progress is changed.
         * @return The server's initialization's progress changed event
         */
        Nickvision::Events::Event<Shared::Events::ServerInitializationProgressChangedEventArgs>& serverInitializationProgressChanged();
        /**
         * @brief Gets the event for when a server is loaded.
         * @return The server loaded event
         */
        Nickvision::Events::Event<Shared::Events::ServerLoadedEventArgs>& serverLoaded();
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
         * @brief This function will also initalize the server as well.
         * @brief This function will call the server loaded event when creation is complete.
         * @param version The version of the server
         * @param properties The properties of the server
         * @return True if successful
         * @return False if the server creation failed.
         */
        bool createServer(const ServerVersion& version, const ServerProperties& properties);
        /**
         * @brief Loads a server.
         * @brief This function will call the server loaded event when loading is complete.
         * @param name The name of the server to load
         * @return True if successful
         * @return False if the server is already loaded
         */
        bool loadServer(const std::string& name);
        /**
         * @brief Deletes a server.
         * @brief The server must not be running to be deleted.
         * @param name The name of the server to delete
         * @return True if deleted
         * @return False is not deleted
         */
        bool deleteServer(const std::string& name);

    private:
        const Configuration& m_configuration;
        std::filesystem::path m_serversDirectory;
        std::unordered_map<std::string, std::shared_ptr<Server>> m_servers;
        std::unordered_map<std::string, bool> m_loadedServers;
        Nickvision::Events::Event<Shared::Events::ServerInitializationProgressChangedEventArgs> m_serverInitializationProgressChanged;
        Nickvision::Events::Event<Shared::Events::ServerLoadedEventArgs> m_serverLoaded;
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<std::string>> serverDeleted;
    };
}

#endif //SERVERMANAGER_H
