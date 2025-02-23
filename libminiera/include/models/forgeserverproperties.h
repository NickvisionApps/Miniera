#ifndef FORGESERVERPROPERTIES_H
#define FORGESERVERPROPERTIES_H

#include <string>
#include "javaserverproperties.h"

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of properties for a Forge Minecraft server.
     */
    class ForgeServerProperties : public JavaServerProperties
    {
    public:
        /**
         * @brief Construct a ForgeServerProperties.
         */
        ForgeServerProperties();
        /**
         * @brief Converts the server properties to a string to save to a file.
         * @return The server properties as a string
         */
        std::string toString() const override;

    private:
        
    };
}

#endif //FORGESERVERPROPERTIES_H