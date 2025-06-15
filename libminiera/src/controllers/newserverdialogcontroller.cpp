#include "controllers/newserverdialogcontroller.h"
#include <thread>
#include <libnick/localization/gettext.h>
#include <libnick/notifications/appnotification.h>

using namespace Nickvision::Events;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Miniera::Shared::Models;
using namespace Nickvision::Notifications;

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

    std::string NewServerDialogController::ensureServerNameIsUnique(const std::string& name)
    {
        int i{ 1 };
        std::string unique{ name };
        while(m_serverManager.getServerExists(unique))
        {
            unique = name + " " + std::to_string(i++);
        }
        return unique;
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

    bool NewServerDialogController::createServer()
    {
        if(m_serverProperties.getLevelName().empty())
        {
            AppNotification::send({ _("The server name cannot be empty"), NotificationSeverity::Error });
            return false;
        }
        else if(m_serverManager.getServerExists(m_serverProperties.getLevelName()))
        {
            AppNotification::send({ _("A server with this name exists"), NotificationSeverity::Error });
            return false;
        }
        else if(!m_serverManager.createServer(m_serverVersions.at(m_selectedServerVersionIndex), m_serverProperties))
        {
            AppNotification::send({ _("An unknown error occurred"), NotificationSeverity::Error });
            return false;
        }
        return true;
    }
}
