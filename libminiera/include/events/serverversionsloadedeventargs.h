#ifndef SERVERVERSIONSLOADEDEVENTARGS_H
#define SERVERVERSIONSLOADEDEVENTARGS_H

#include <vector>
#include <libnick/events/eventargs.h>
#include "models/serverversion.h"

namespace Nickvision::Miniera::Shared::Events
{
    /**
     * @brief Event arguments for when server versions are loaded.
     */
    class ServerVersionsLoadedEventArgs : public Nickvision::Events::EventArgs
    {
    public:
        /**
         * @brief Constructs a ServerVersionsLoadedEventArgs.
         * @param versions The loaded server versions
         */
        ServerVersionsLoadedEventArgs(const std::vector<Models::ServerVersion>& versions);
        /**
         * @brief Gets the loaded server versions.
         * @return The loaded server versions
         */
        const std::vector<Models::ServerVersion>& getVersions() const;
    
    private:
        std::vector<Models::ServerVersion> m_versions;
    };
}

#endif //SERVERVERSIONSLOADEDEVENTARGS_H