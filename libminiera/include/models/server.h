#ifndef SERVER_H
#define SERVER_H

#include <filesystem>
#include <string>
#include <boost/json.hpp>
#include <libnick/events/event.h>
#include <libnick/network/curleasy.h>
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
         * @return True if initialization successful
         * @return False if initialization failed
         */
        bool initialize();
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
        /**
         * @brief Handles when the initaliazation download progress changes.
         * @param dltotal The total number of bytes to download
         * @param dlnow The total number of bytes downloaded
         * @param utotal The total number of bytes to upload
         * @param unow The total number of bytes uploaded
         * @return 0
         */
        int onInitializationDownloadProgressChanged(curl_off_t dltotal, curl_off_t dlnow, curl_off_t utotal, curl_off_t unow);
        ServerVersion m_serverVersion;
        ServerProperties m_serverProperties;
        std::filesystem::path m_serverDirectory;
        bool m_isInitialized;
    };
}

#endif //SERVER_H
