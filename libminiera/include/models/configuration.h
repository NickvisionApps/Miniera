#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <libnick/app/datafilebase.h>
#include <libnick/app/windowgeometry.h>
#include "theme.h"

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model for the configuration of the application.
     */
    class Configuration : public Nickvision::App::DataFileBase
    {
    public:
        /**
         * @brief Constructs a Configuration.
         * @param key The key to pass to the DataFileBase
         * @param appName The appName to pass to the DataFileBase
         */
        Configuration(const std::string& key, const std::string& appName);
        /**
         * @brief Gets the preferred theme for the application.
         * @return The preferred theme
         */
        Theme getTheme() const;
        /**
         * @brief Sets the preferred theme for the application.
         * @param theme The new preferred theme
         */
        void setTheme(Theme theme);
        /**
         * @brief Gets the window geometry for the application.
         * @return The window geometry
         */
        App::WindowGeometry getWindowGeometry() const;
        /**
         * @brief Sets the window geometry for the application.
         * @param geometry The new window geometry
         */
        void setWindowGeometry(const App::WindowGeometry& geometry);
        /**
         * @brief Gets whether or not to automatically check for application updates.
         * @return True to automatically check for updates, else false
         */
        bool getAutomaticallyCheckForUpdates() const;
        /**
         * @brief Sets whether or not to automatically check for application updates.
         * @param check Whether or not to automatically check for updates
         */
        void setAutomaticallyCheckForUpdates(bool check);
        /**
         * @brief Gets the maximum amount of RAM a server can use in GB.
         * @return The maximum amount of RAM a server can use
         */
        unsigned int getMaxServerRamInGB() const;
        /**
         * @brief Sets the maximum amount of RAM a server can use in GB.
         * @param ram The new maximum amount of RAM a server can use
         */
        void setMaxServerRamInGB(unsigned int ram);
        /**
         * @brief Gets whether or not to delete the source mod file after upload.
         * @return True to delete source mod file, else false
         */
        bool getDeleteModAfterUpload() const;
        /**
         * @brief Sets whether or not to delete the source mod file after upload.
         * @param deleteMod Whether or not to delete the source mod file after upload
         */
        void setDeleteModAfterUpload(bool deleteMod);
        /**
         * @brief Gets the auth token for ngrok.
         * @return The auth token for ngrok
         */
        std::string getNgrokAuthToken() const;
        /**
         * @brief Sets the auth token for ngrok.
         * @param token The new auth token for ngrok
         */
        void setNgrokAuthToken(const std::string& token);
    };
}

#endif //CONFIGURATION_H
