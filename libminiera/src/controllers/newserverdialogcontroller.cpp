#include "controllers/newserverdialogcontroller.h"
#include <thread>

using namespace Nickvision::Events;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Miniera::Shared::Models;

namespace Nickvision::Miniera::Shared::Controllers
{
    NewServerDialogController::NewServerDialogController()
        : m_serverProperties{ Edition::Java },
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

    void NewServerDialogController::loadServerVersions(Edition edition)
    {
        std::thread worker{ [this, edition]()
        {
            m_serverVersions = ServerVersion::fetch(edition);
            m_serverVersionsLoaded.invoke({ m_serverVersions });
        } };
        worker.detach();
    }
}