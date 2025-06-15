#ifndef EDITION_H
#define EDITION_H

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief Types of Minecraft server editions.
     */
    enum class Edition
    {
        Java,
        Bedrock,
        Forge
    };
}

#endif //EDITION_H