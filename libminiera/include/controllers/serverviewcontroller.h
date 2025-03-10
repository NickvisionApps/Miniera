#ifndef SERVERVIEWCONTROLLER_H
#define SERVERVIEWCONTROLLER_H

#include <memory>
#include <string>
#include <thread>
#include <libnick/events/event.h>
#include <libnick/events/parameventargs.h>
#include "models/configuration.h"
#include "models/powerstatus.h"
#include "models/server.h"

namespace Nickvision::Miniera::Shared::Controllers
{
    /**
     * @brief A controller for a server view.
     */
    class ServerViewController
    {
    public:
        /**
         * @brief Constructs a ServerViewController.
         * @param server The server to manage
         * @param configuration The app's configuration
         * @throw std::invalid_argument Thrown if the server is nullptr
         */
        ServerViewController(const std::shared_ptr<Models::Server>& server, const Models::Configuration& configuration);
        /**
         * @brief Destructs a ServerViewController.
         */
        ~ServerViewController();
        /**
         * @brief Gets the event for when the console output changes.
         * @return The console output changed event
         */
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<std::string>>& consoleOutputChanged();
        /**
         * @brief Gets the name of the server.
         * @return The name of the server
         */
        const std::string& getName() const;
        /**
         * @brief Gets the version of the server.
         * @return The version of the server
         */
        std::string getVersion() const;
        /**
         * @brief Gets whether or not the server supports mods.
         * @return True if the server supports mods
         * @return False if the server does not support mods
         */
        bool supportsMods() const;
        /**
         * @brief Starts a server if it is stopped or stops a server if it is started.
         * @return PowerStatus
         */
        Models::PowerStatus startStop();
        /**
         * @brief Sends a command to the server.
         * @param cmd The command to send to the server
         */
        void sendCommand(const std::string& cmd);

    private:
        /**
         * @brief Watches a running server.
         */
        void watch();
        std::shared_ptr<Models::Server> m_server;
        const Models::Configuration& m_configuration;
        std::thread m_watcher;
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<std::string>> m_consoleOutputChanged;
    };
}

#endif //SERVERVIEWCONTROLLER_H
