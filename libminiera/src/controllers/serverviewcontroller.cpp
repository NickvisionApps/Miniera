#include "controllers/serverviewcontroller.h"
#include <stdexcept>

using namespace Nickvision::Miniera::Shared::Models;

namespace Nickvision::Miniera::Shared::Controllers
{
    ServerViewController::ServerViewController(const std::shared_ptr<Server>& server, ServerManager& serverManager)
        : m_server{ server },
        m_serverManager{ serverManager },
        m_started{ false }
    {
        if(!server)
        {
            throw std::invalid_argument("Server cannot be null");
        }
    }

    const std::string& ServerViewController::getName() const
    {
        return m_server->getName();
    }

    std::string ServerViewController::getVersion() const
    {
        return m_server->getVersion().str();
    }

    void ServerViewController::startup()
    {
        if(!m_started)
        {
            m_server->initialize();
            m_started = true;
        }
    }
}
