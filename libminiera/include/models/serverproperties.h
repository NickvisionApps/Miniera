#ifndef SERVERPROPERTIES_H
#define SERVERPROPERTIES_H

#include <string>
#include "difficulty.h"
#include "edition.h"
#include "gamemode.h"

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of properties for a Minecraft server.
     */
    class ServerProperties
    {
    public:
        virtual ~ServerProperties() = 0;
        /**
         * @brief Constructs a ServerProperties.
         * @param edition The edition of the server
         */
        ServerProperties(Edition edition);
        /**
         * @brief Gets the difficulty of the server.
         * @return The difficulty of the server
         */
        Difficulty getDifficulty() const;
        /**
         * @brief Sets the difficulty of the server.
         * @param difficulty The new difficulty of the server
         */
        void setDifficulty(Difficulty difficulty);
        /**
         * @brief Gets whether the server forces a gamemode.
         * @return Whether the server forces a gamemode
         */
        bool getForceGamemode() const;
        /**
         * @brief Sets whether the server forces a gamemode.
         * @param forceGamemode Whether the server forces a gamemode
         */
        void setForceGamemode(bool forceGamemode);
        /**
         * @brief Gets the gamemode of the server.
         * @return The gamemode of the server
         */
        Gamemode getGamemode() const;
        /**
         * @brief Sets the gamemode of the server.
         * @param gamemode The new gamemode of the server
         */
        void setGamemode(Gamemode gamemode);
        /**
         * @brief Gets the name of the level.
         * @return The name of the level
         */
        const std::string& getLevelName() const;
        /**
         * @brief Sets the name of the level.
         * @param levelName The new name of the level
         */
        void setLevelName(const std::string& levelName);
        /**
         * @brief Gets the seed of the level.
         * @return The seed of the level
         */
        const std::string& getLevelSeed() const;
        /**
         * @brief Sets the seed of the level.
         * @param levelSeed The new seed of the level
         */
        void setLevelSeed(const std::string& levelSeed);
        /**
         * @brief Gets the maximum number of players.
         * @return The maximum number of players
         */
        unsigned int getMaxPlayers() const;
        /**
         * @brief Sets the maximum number of players.
         * @param maxPlayers The new maximum number of players
         */
        void setMaxPlayers(unsigned int maxPlayers);
        /**
         * @brief Gets whether the server is in online mode.
         * @return Whether the server is in online mode
         */
        bool getOnlineMode() const;
        /**
         * @brief Sets whether the server is in online mode.
         * @param onlineMode Whether the server is in online mode
         */
        void setOnlineMode(bool onlineMode);
        /**
         * @brief Gets the player idle timeout.
         * @return The player idle timeout
         */
        unsigned int getPlayerIdleTimeout() const;
        /**
         * @brief Sets the player idle timeout.
         * @param playerIdleTimeout The new player idle timeout
         */
        void setPlayerIdleTimeout(unsigned int playerIdleTimeout);
        /**
         * @brief Gets the server port.
         * @return The server port
         */
        unsigned int getServerPort() const;
        /**
         * @brief Sets the server port.
         * @param serverPort The new server port
         */
        void setServerPort(unsigned int serverPort);
        /**
         * @brief Gets the view distance.
         * @return The view distance
         */
        unsigned int getViewDistance() const;
        /**
         * @brief Sets the view distance.
         * @param viewDistance The new view distance
         */
        void setViewDistance(unsigned int viewDistance);
        /**
         * @brief Converts the server properties to a string to save to a file.
         * @return The server properties as a string
         */
        virtual std::string toString() const;

    protected:
        Edition m_edition;
        Difficulty m_difficulty;
        bool m_forceGamemode;
        Gamemode m_gamemode;
        std::string m_levelName;
        std::string m_levelSeed;
        unsigned int m_maxPlayers;
        bool m_onlineMode;
        unsigned int m_playerIdleTimeout;
        unsigned int m_serverPort;
        unsigned int m_viewDistance;
    };
}

#endif //SERVERPROPERTIES_H