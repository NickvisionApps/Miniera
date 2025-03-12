#ifndef SERVERVIEWCONTROLLER_H
#define SERVERVIEWCONTROLLER_H

#include <memory>
#include <string>
#include <thread>
#include <utility>
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
         * @brief Gets the event for when the resource usage changes.
         * @return The resource usage changed event
         */
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<std::pair<double, unsigned long long>>>& resourceUsageChanged();
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
         * @brief Gets the address of the server.
         * @return The address of the server
         */
        Models::ServerAddress getAddress() const;
        /**
         * @brief Gets whether or not the server supports mods.
         * @return True if the server supports mods
         * @return False if the server does not support mods
         */
        bool supportsMods() const;
        /**
         * @brief Gets the ram usage string.
         * @param bytes The number of bytes being used by the server
         * @return The ram usage string
         */
        std::string getRAMString(unsigned long long bytes);
        /**
         * @brief Starts a server if it is stopped or stops a server if it is started.
         * @return PowerStatus
         */
        Models::PowerStatus startStop();
        /**
         * @brief Broadcasts the server over the WWW.
         * @brief On success, getAddress() should be called to display the new address.
         * @return True if successful
         * @return False if unsuccesful
         */
        bool broadcast();
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
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<std::pair<double, unsigned long long>>> m_resourceUsageChanged;
    };
}

#endif //SERVERVIEWCONTROLLER_H
