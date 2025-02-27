#ifndef SERVERVIEWCONTROLLER_H
#define SERVERVIEWCONTROLLER_H

#include <memory>
#include <string>
#include "models/server.h"
#include "models/servermanager.h"

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
         * @param serverManager The server manager
         * @throw std::invalid_argument Thrown if the server is nullptr
         */
        ServerViewController(const std::shared_ptr<Models::Server>& server, Models::ServerManager& serverManager);
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

    private:
        std::shared_ptr<Models::Server> m_server;
        Models::ServerManager& m_serverManager;
    };
}

#endif //SERVERVIEWCONTROLLER_H