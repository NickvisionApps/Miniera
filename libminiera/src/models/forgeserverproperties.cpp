#include "models/forgeserverproperties.h"
#include <sstream>

namespace Nickvision::Miniera::Shared::Models
{
    ForgeServerProperties::ForgeServerProperties()
        : JavaServerProperties{ Edition::Forge }
    {

    }

    std::string ForgeServerProperties::toString() const
    {
        std::stringstream builder;
        builder << JavaServerProperties::toString();
        return builder.str();
    }
}