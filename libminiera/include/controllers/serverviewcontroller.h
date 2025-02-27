#ifndef SERVERVIEWCONTROLLER_H
#define SERVERVIEWCONTROLLER_H

#include <memory>
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
         */
        ServerViewController(const std::shared_ptr<Models::Server>& server, Models::ServerManager& serverManager);

    private:
        std::shared_ptr<Models::Server> m_server;
        Models::ServerManager& m_serverManager;
    };
}

#endif //SERVERVIEWCONTROLLER_H