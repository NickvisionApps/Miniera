#include "controllers/serverviewcontroller.h"

using namespace Nickvision::Miniera::Shared::Models;

namespace Nickvision::Miniera::Shared::Controllers
{
    ServerViewController::ServerViewController(const std::shared_ptr<Server>& server, ServerManager& serverManager)
        : m_server{ server }, 
        m_serverManager{ serverManager }
    {

    }
}