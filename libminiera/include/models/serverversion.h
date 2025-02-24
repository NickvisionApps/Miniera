#ifndef SERVERVERSION_H
#define SERVERVERSION_H

#include <filesystem>
#include <string>
#include <vector>
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
         * @brief Downloads the server files.
         * @param path The path to save the server files
         * @return True if the server jar was downloaded successfully, false otherwise
         */
        bool downloadServer(std::filesystem::path path) const;

    private:
        Edition m_edition;
        Update::Version m_version;
        std::string m_releaseUrl;
    };
}

#endif //SERVERVERSION_H