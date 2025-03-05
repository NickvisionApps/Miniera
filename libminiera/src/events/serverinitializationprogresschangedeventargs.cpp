#include "events/serverinitializationprogresschangedeventargs.h"

namespace Nickvision::Miniera::Shared::Events
{
    ServerInitializationProgressChangedEventArgs::ServerInitializationProgressChangedEventArgs(const std::string& name, double progress, const std::string& log, bool isFinished, bool isError)
        : m_name{ name },
        m_progress{ progress },
        m_log{ log },
        m_isFinished{ isFinished },
        m_isError{ isError }
    {

    }

    const std::string& ServerInitializationProgressChangedEventArgs::getName() const
    {
        return m_name;
    }

    double ServerInitializationProgressChangedEventArgs::getProgress() const
    {
        return m_progress;
    }

    const std::string& ServerInitializationProgressChangedEventArgs::getLog() const
    {
        return m_log;
    }

    bool ServerInitializationProgressChangedEventArgs::isFinished() const
    {
        return m_isFinished;
    }

    bool ServerInitializationProgressChangedEventArgs::isError() const
    {
        return m_isError;
    }
}
