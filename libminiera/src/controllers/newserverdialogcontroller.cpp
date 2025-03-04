#include "controllers/newserverdialogcontroller.h"
#include <thread>

using namespace Nickvision::Events;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Miniera::Shared::Models;

namespace Nickvision::Miniera::Shared::Controllers
{
    NewServerDialogController::NewServerDialogController(ServerManager& serverManager)
        : m_serverManager{ serverManager }, 
        m_serverProperties{ Edition::Java },
        m_selectedServerVersionIndex{ 0 }
    {

    }

    Event<ServerVersionsLoadedEventArgs>& NewServerDialogController::serverVersionsLoaded()
    {
        return m_serverVersionsLoaded;
    }

    const ServerProperties& NewServerDialogController::getServerProperties() const
    {
        return m_serverProperties;
    }

    void NewServerDialogController::setServerProperties(const ServerProperties& serverProperties)
    {
        m_serverProperties = serverProperties;
    }

    const ServerVersion& NewServerDialogController::getSelectedServerVersion() const
    {
        return m_serverVersions.at(m_selectedServerVersionIndex);
    }

    void NewServerDialogController::setSelectedServerVersionIndex(size_t index)
    {
        m_selectedServerVersionIndex = index;
    }

    std::string NewServerDialogController::ensureServerNameIsUnique(std::string name)
    {
        int i{ 1 };
        while(m_serverManager.getServerExists(name))
        {
            name = name + " " + std::to_string(i++);
        }
        return name;
    }
    
    void NewServerDialogController::loadServerVersions(Edition edition)
    {
        std::thread worker{ [this, edition]()
        {
            m_serverVersions = ServerVersion::fetch(edition);
            m_serverVersionsLoaded.invoke({ m_serverVersions });
        } };
        worker.detach();
    }

    ServerCheckStatus NewServerDialogController::createServer()
    {
        if(m_serverProperties.getLevelName().empty())
        {
            return ServerCheckStatus::EmptyName;
        }
        else if(m_serverManager.getServerExists(m_serverProperties.getLevelName()))
        {
            return ServerCheckStatus::ExistingName;
        }
        else if(!m_serverManager.createServer(m_serverVersions.at(m_selectedServerVersionIndex), m_serverProperties))
        {
            return ServerCheckStatus::CreateError;
        }
        return ServerCheckStatus::Valid;
    }
}
