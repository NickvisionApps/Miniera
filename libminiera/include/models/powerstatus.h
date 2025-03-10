#ifndef POWERSTATUS_H
#define POWERSTATUS_H

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief Statuses for when a server's power state is toggled.
     */
    enum class PowerStatus
    {
        Started,
        ErrorStarting,
        Stopped,
        ErrorStopping
    };
}

#endif //POWERSTATUS_H
