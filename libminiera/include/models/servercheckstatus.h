#ifndef SERVERCHECKSTATUS_H
#define SERVERCHECKSTATUS_H

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief Statuses for when a new server is validated.
     */
    enum class ServerCheckStatus
    {
        Valid = 1,
        EmptyName = 2,
        ExistingName = 4,
        CreateError = 8
    };
}

#endif //SERVERCHECKSTATUS_H