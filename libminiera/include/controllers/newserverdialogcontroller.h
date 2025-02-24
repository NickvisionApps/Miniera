#ifndef NEWSERVERDIALOGCONTROLLER_H
#define NEWSERVERDIALOGCONTROLLER_H

#include <vector>
#include <libnick/events/event.h>
#include "events/serverversionsloadedeventargs.h"
#include "models/edition.h"
#include "models/servercheckstatus.h"
#include "models/servermanager.h"
#include "models/serverproperties.h"
#include "models/serverversion.h"

namespace Nickvision::Miniera::Shared::Controllers
{
    /**
     * @brief A controller for a New Server dialog.
     */
    class NewServerDialogController
    {
    public:
        /**
         * @brief Constructs a NewServerDialogController.
         * @param serverManager The server manager
         */
        NewServerDialogController(Models::ServerManager& serverManager);
        /**
         * @brief Gets the event for when the server versions are loaded.
         * @return The server versions loaded event
         */
        Nickvision::Events::Event<Events::ServerVersionsLoadedEventArgs>& serverVersionsLoaded();
        /**
         * @brief Gets the server properties.
         * @return The server properties
         */
        const Models::ServerProperties& getServerProperties() const;
        /**
         * @brief Sets the server properties.
         * @param serverProperties The new server properties
         */
        void setServerProperties(const Models::ServerProperties& serverProperties);
        /**
         * @brief Gets the selected server version.
         * @return The selected server version
         */
        const Models::ServerVersion& getSelectedServerVersion() const;
        /**
         * @brief Sets the index of the selected server version.
         * @param index The index of the selected server version
         */
        void setSelectedServerVersionIndex(size_t index);
        /**
         * @brief Loads the server versions.
         * @param edition The server edition to load servers for
         */
        void loadServerVersions(Models::Edition edition);
        /**
         * @brief Creates the new server.
         * @return The ServerCheckStatus
         */
        Models::ServerCheckStatus createServer();
    
    private:
        Models::ServerManager& m_serverManager;
        Models::ServerProperties m_serverProperties;
        std::vector<Models::ServerVersion> m_serverVersions;
        size_t m_selectedServerVersionIndex;
        Nickvision::Events::Event<Events::ServerVersionsLoadedEventArgs> m_serverVersionsLoaded;
    };
}

#endif //NEWSERVERDIALOGCONTROLLER_H