#include "events/serverloadedeventargs.h"

using namespace Nickvision::Miniera::Shared::Controllers;

namespace Nickvision::Miniera::Shared::Events
{
    ServerLoadedEventArgs::ServerLoadedEventArgs(const std::string& name, const std::shared_ptr<ServerViewController>& serverViewController)
        : m_name{ name },
        m_serverViewController{ serverViewController }
    {
        
    }

    const std::string& ServerLoadedEventArgs::getName() const
    {
        return m_name;
    }

    const std::shared_ptr<ServerViewController>& ServerLoadedEventArgs::getServerViewController() const
    {
        return m_serverViewController;
    }
}