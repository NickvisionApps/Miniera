#ifndef PREFERENCESVIEWCONTROLLER_H
#define PREFERENCESVIEWCONTROLLER_H

#include <string>
#include "models/configuration.h"

namespace Nickvision::Miniera::Shared::Controllers
{
    /**
     * @brief A controller for a PreferencesView.
     */
    class PreferencesViewController
    {
    public:
        /**
         * @brief Constructs a PreferencesViewController.
         * @param configuration The reference to the configuration to use
         */
        PreferencesViewController(Models::Configuration& configuration);
        /**
         * @brief Gets the preferred theme for the application.
         * @return The preferred theme
         */
        Models::Theme getTheme() const;
        /**
         * @brief Sets the preferred theme for the application.
         * @param theme The new preferred theme
         */
        void setTheme(Models::Theme theme);
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
         * @brief Gets the auth token for ngrok.
         * @return The auth token for ngrok
         */
        std::string getNgrokAuthToken() const;
        /**
         * @brief Sets the auth token for ngrok.
         * @param token The new auth token for ngrok
         */
        void setNgrokAuthToken(const std::string& token);
        /**
         * @brief Gets the URL to retrieve the ngrok auth token.
         * @return The URL to retrieve the ngrok auth token
         */
        const std::string& getNgrokAuthTokenRetrievalUrl() const;
        /**
         * @brief Saves the current configuration to disk.
         */
        void saveConfiguration();

    private:
        Models::Configuration& m_configuration;
    };
}

#endif //PREFERENCESVIEWCONTROLLER_H