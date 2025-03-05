#ifndef SERVER_H
#define SERVER_H

#include <filesystem>
#include <string>
#include <boost/json.hpp>
#include <libnick/events/event.h>
#include <libnick/network/curleasy.h>
#include "events/serverinitializationprogresschangedeventargs.h"
#include "serverproperties.h"
#include "serverversion.h"

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of a minecraft server.
     */
    class Server
    {
    public:
        /**
         * @brief Constructs a Server.
         * @param serverVersion The version of the server
         * @param serverProperties The properties of the server
         * @param serverDirectory The directory of the server
         */
        Server(const ServerVersion& serverVersion, const ServerProperties& serverProperties, const std::filesystem::path& serverDirectory);
        /**
         * @brief Constructs a Server.
         * @param json The json object to construct the server from
         * @throw std::invalid_argument Thrown if the json object is invalid
         */
        Server(boost::json::object json);
        /**
         * @brief Gets the event for when the initialization's progress is changed.
         * @return The initialization progress changed event.
         */
        Nickvision::Events::Event<Events::ServerInitializationProgressChangedEventArgs>& initializationProgressChanged();
        /**
         * @brief Gets the name of the server.
         * @return The name of the server
         */
        const std::string& getName() const;
        /**
         * @brief Gets the version of the server.
         * @return The version of the server
         */
        const ServerVersion& getVersion() const;
        /**
         * @brief Gets whether or not the server has been initialized (downloaded server files).
         * @return True if initialized
         * @return False if not initialized
         */
        bool isInitialized() const;
        /**
         * @brief Initializes the server.
         * @brief This will download and setup the main server executable and directories.
         * @brief This function runs on another thread. Progress should be tracked via the ServerInitializationProgressChanged event.
         */
        void initialize();
        /**
         * @brief Converts the Server to a JSON object.
         * @return The Server as a JSON object
         */
        boost::json::object toJson() const;
        /**
         * @brief Compares this Server to other Server via ==.
         * @param other The other Server
         * @return True if this == other
         */
        bool operator==(const Server& other) const;
        /**
         * @brief Compares this Server to other Server via !=.
         * @param other The other Server
         * @return True if this != other
         */
        bool operator!=(const Server& other) const;

    private:
        /**
         * @brief Writes the server json to disk.
         */
        void writeJsonToDisk() const;
        ServerVersion m_serverVersion;
        ServerProperties m_serverProperties;
        std::filesystem::path m_serverDirectory;
        bool m_isInitialized;
        Nickvision::Events::Event<Events::ServerInitializationProgressChangedEventArgs> m_initializationProgressChanged;
    };
}

#endif //SERVER_H
