#ifndef SERVERLOADEDEVENTARGS_H
#define SERVERLOADEDEVENTARGS_H

#include <memory>
#include <string>
#include <libnick/events/eventargs.h>
#include "controllers/serverviewcontroller.h"

namespace Nickvision::Miniera::Shared::Events
{
    /**
     * @brief Event arguments for when a server is loaded.
     */
    class ServerLoadedEventArgs : public Nickvision::Events::EventArgs
    {
    public:
        /**
         * @brief Constructs a ServerLoadedEventArgs.
         * @param name The name of the loaded server
         * @param serverViewController The ServerViewController to use for the loaded server
         */
        ServerLoadedEventArgs(const std::string& name, const std::shared_ptr<Controllers::ServerViewController>& serverViewController);
        /**
         * @brief Gets the name of the loaded server.
         * @return The name of the loaded server
         */
        const std::string& getName() const;
        /**
         * @brief Gets the ServerViewController.
         * @return The ServerViewController
         */
        const std::shared_ptr<Controllers::ServerViewController>& getServerViewController() const;

    private:
        std::string m_name;
        std::shared_ptr<Controllers::ServerViewController> m_serverViewController;
    };
}

#endif //SERVERLOADEDEVENTARGS_H
