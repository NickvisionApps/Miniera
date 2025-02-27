#ifndef SERVER_H
#define SERVER_H

#include <filesystem>
#include <string>
#include <boost/json.hpp>
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
    };
}

#endif //SERVER_H