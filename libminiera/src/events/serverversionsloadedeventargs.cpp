#include "events/serverversionsloadedeventargs.h"

using namespace Nickvision::Miniera::Shared::Models;

namespace Nickvision::Miniera::Shared::Events
{
    ServerVersionsLoadedEventArgs::ServerVersionsLoadedEventArgs(const std::vector<ServerVersion>& versions)
        : m_versions{ versions }
    {

    }

    const std::vector<ServerVersion>& ServerVersionsLoadedEventArgs::getVersions() const
    {
        return m_versions;
    }
}