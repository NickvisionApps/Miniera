#ifndef SERVERVERSION_H
#define SERVERVERSION_H

#include <filesystem>
#include <string>
#include <vector>
#include <boost/json.hpp>
#include <libnick/update/version.h>
#include "edition.h"

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of a server version.
     */
    class ServerVersion
    {
    public:
        /**
         * @brief Constructs a ServerVersion.
         * @param edition The edition of the server
         * @param version The version of the server
         * @param releaseUrl The URL to the release json
         */
        ServerVersion(Edition edition, const Update::Version& version, const std::string& releaseUrl);
        /**
         * @brief Constructs a ServerVersion.
         * @param json The JSON object to construct the ServerVersion from
         * @throw std::invalid_argument Thrown if the JSON object is invalid
         */
        ServerVersion(boost::json::object json);
        /**
         * @brief Fetches all server versions for a given edition.
         * @param edition The edition of the server
         * @return The list of server versions
         */
        static std::vector<ServerVersion> fetch(Edition edition);
        /**
         * @brief Gets the edition of the server.
         * @return The edition of the server
         */
        Edition getEdition() const;
        /**
         * @brief Gets the version of the server.
         * @return The version of the server
         */
        const Update::Version& getVersion() const;
        /**
         * @brief Gets the URL to the release json.
         * @return The URL to the release json
         */
        const std::string& getReleaseUrl() const;
        /**
         * @brief Downloads the server.
         * @param path The path (or root directory) to save the server to
         * @return The final path of the downloaded server file
         */
        std::filesystem::path downloadServer(std::filesystem::path path) const;
        /**
         * @brief Converts the ServerVersion to a string.
         * @return The ServerVersion as a string
         */
        std::string str() const;
        /**
         * @brief Converts the ServerVersion to a JSON object.
         * @return The ServerVersion as a JSON object
         */
        boost::json::object toJson() const;
        /**
         * @brief Compares this ServerVersion with other via ==.
         * @param other The ServerVersion to compare to
         * @return True if this == other, else false
         */
        bool operator==(const ServerVersion& other) const;
        /**
         * @brief Compares this ServerVersion with other via !=.
         * @param other The ServerVersion to compare to
         * @return True if this != other, else false
         */
        bool operator!=(const ServerVersion& other) const;

    private:
        Edition m_edition;
        Update::Version m_version;
        std::string m_releaseUrl;
    };
}

#endif //SERVERVERSION_H
