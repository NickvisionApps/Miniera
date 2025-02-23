#ifndef GAMEMODE_H
#define GAMEMODE_H

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief Types of Minecraft gamemodes.
     */
    enum class Gamemode
    {
        Survival = 0,
        Creative,
        Adventure
    };
}

#endif //GAMEMODE_H