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
         * @brief Gets the event from when the server's power status changes.
         * @return The power changed event
         */
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<Models::PowerStatus>>& powerChanged();
        /**
         * @brief Gets the event for when the server's address changes.
         * @return The address changed event
         */
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<Models::ServerAddress>>& addressChanged();
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
         * @brief Gets whether or not the server is running.
         * @return True if the server is running, else false
         */
        bool isRunning() const;
        /**
         * @brief Gets whether or not the server supports mods.
         * @return True if the server supports mods
         * @return False if the server does not support mods
         */
        bool supportsMods() const;
        /**
         * @brief Gets the list of all mods in the server.
         * @return The list of server mods
         */
        const std::vector<std::string>& getMods() const;
        /**
         * @brief Gets the maximum amount of RAM a server can use in MB.
         * @return The maximum amount of RAM a server can use
         */
        unsigned long getMaxServerRamInMB() const;
        /**
         * @brief Gets the ram usage string.
         * @param bytes The number of bytes being used by the server
         * @return The ram usage string
         */
        std::string getRAMString(unsigned long long bytes) const;
        /**
         * @brief Starts a server if it is stopped or stops a server if it is started.
         * @brief This method will trigger the powerChanged event as the status changes.
         */
        void startStop();
        /**
         * @brief Broadcasts the server over the WWW.
         * @brief This method will trigger the addressChanged event as the address changes.
         */
        void broadcast();
        /**
         * @brief Sends a command to the server.
         * @param cmd The command to send to the server
         */
        void sendCommand(const std::string& cmd);
        /**
         * @brief Uploads a mod to the server.
         * @param file The path to the mod's jar file
         * @return True if the mod was added
         * @return False if the mod was not added
         */
        bool uploadMod(const std::filesystem::path& file);
        /**
         * @brief Removes a mod from the server.
         * @param mod The mod to delete
         * @return True if the mod was removed
         * @return False if the mod was not removed
         */
        bool removeMod(const std::string& mod);

    private:
        /**
         * @brief Watches a running server.
         */
        void watch();
        std::shared_ptr<Models::Server> m_server;
        const Models::Configuration& m_configuration;
        std::thread m_watcher;
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<Models::PowerStatus>> m_powerChanged;
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<Models::ServerAddress>> m_addressChanged;
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<std::string>> m_consoleOutputChanged;
        Nickvision::Events::Event<Nickvision::Events::ParamEventArgs<std::pair<double, unsigned long long>>> m_resourceUsageChanged;
    };
}

#endif //SERVERVIEWCONTROLLER_H
