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
         * @brief Gets whether or not flight is allowed.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not flight is allowed
         */
        bool getAllowFlight() const;
        /**
         * @brief Sets whether or not flight is allowed.
         * @brief This property only effects Java and Forge editions.
         * @param allowFlight Whether or not flight is allowed
         */
        void setAllowFlight(bool allowFlight);
        /**
         * @brief Gets whether or not the Nether is allowed.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not the Nether is allowed
         */
        bool getAllowNether() const;
        /**
         * @brief Sets whether or not the Nether is allowed.
         * @brief This property only effects Java and Forge editions.
         * @param allowNether Whether or not the Nether is allowed
         */
        void setAllowNether(bool allowNether);
        /**
         * @brief Gets whether or not command blocks are enabled.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not command blocks are enabled
         */ 
        bool getEnableCommandBlock() const;
        /**
         * @brief Sets whether or not command blocks are enabled.
         * @brief This property only effects Java and Forge editions.
         * @param enableCommandBlock Whether or not command blocks are enabled
         */
        void setEnableCommandBlock(bool enableCommandBlock);
        /**
         * @brief Gets whether or not structures are generated.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not structures are generated
         */
        bool getGenerateStructures() const;
        /**
         * @brief Sets whether or not structures are generated.
         * @brief This property only effects Java and Forge editions.
         * @param generateStructures Whether or not structures are generated
         */
        void setGenerateStructures(bool generateStructures);
        /**
         * @brief Gets whether or not hardcore mode is enabled.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not hardcore mode is enabled
         */
        bool getHardcore() const;
        /**
         * @brief Sets whether or not hardcore mode is enabled.
         * @brief This property only effects Java and Forge editions.
         * @param hardcore Whether or not hardcore mode is enabled
         */
        void setHardcore(bool hardcore);
        /**
         * @brief Gets whether or not PVP is enabled.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not PVP is enabled
         */
        bool getPVP() const;
        /**
         * @brief Sets whether or not PVP is enabled.
         * @brief This property only effects Java and Forge editions.
         * @param pvp Whether or not PVP is enabled
         */
        void setPVP(bool pvp);
        /**
         * @brief Gets whether or not animals spawn.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not animals spawn
         */
        bool getSpawnAnimals() const;
        /**
         * @brief Sets whether or not animals spawn.
         * @brief This property only effects Java and Forge editions.
         * @param spawnAnimals Whether or not animals spawn
         */
        void setSpawnAnimals(bool spawnAnimals);
        /**
         * @brief Gets whether or not monsters spawn.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not monsters spawn
         */
        bool getSpawnMonsters() const;
        /**
         * @brief Sets whether or not monsters spawn.
         * @brief This property only effects Java and Forge editions.
         * @param spawnMonsters Whether or not monsters spawn
         */
        void setSpawnMonsters(bool spawnMonsters);
        /**
         * @brief Gets whether or not NPCs spawn.
         * @brief This property only effects Java and Forge editions.
         * @return Whether or not NPCs spawn
         */
        bool getSpawnNPCs() const;
        /**
         * @brief Sets whether or not NPCs spawn.
         * @brief This property only effects Java and Forge editions.
         * @param spawnNPCs Whether or not NPCs spawn
         */
        void setSpawnNPCs(bool spawnNPCs);
        /**
         * @brief Gets whether or not cheats are allowed.
         * @brief This property only effects Bedrock editions.
         * @return Whether or not cheats are allowed
         */
        bool getAllowCheats() const;
        /**
         * @brief Sets whether or not cheats are allowed.
         * @brief This property only effects Bedrock editions.
         * @param allowCheats Whether or not cheats are allowed
         */
        void setAllowCheats(bool allowCheats);
        /**
         * @brief Gets whether or not client-side chunk generation is enabled.
         * @brief This property only effects Bedrock editions.
         * @return Whether or not client-side chunk generation is enabled
         */
        bool getClientSideChunkGenerationEnabled() const;
        /**
         * @brief Sets whether or not client-side chunk generation is enabled.
         * @brief This property only effects Bedrock editions.
         * @param clientSideChunkGenerationEnabled Whether or not client-side chunk generation is enabled
         */
        void setClientSideChunkGenerationEnabled(bool clientSideChunkGenerationEnabled);
        /**
         * @brief Gets whether or not custom skins are disabled.
         * @brief This property only effects Bedrock editions.
         * @return Whether or not custom skins are
         */
        bool getDisableCustomSkins() const;
        /**
         * @brief Sets whether or not custom skins are disabled.
         * @brief This property only effects Bedrock editions.
         * @param disableCustomSkins Whether or not custom skins are disabled
         */
        void setDisableCustomSkins(bool disableCustomSkins);
        /**
         * @brief Gets the tick distance.
         * @brief This property only effects Bedrock editions.
         * @return The tick distance
         */
        unsigned int getTickDistance() const;
        /**
         * @brief Sets the tick distance.
         * @brief This property only effects Bedrock editions.
         * @param tickDistance The tick distance
         */
        void setTickDistance(unsigned int tickDistance);
        /**
         * @brief Converts the server properties to a string to save to a file.
         * @return The server properties as a string
         */
        std::string toString() const;

    private:
        //Shared
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
        //Java
        bool m_allowFlight;
        bool m_allowNether;
        bool m_enableCommandBlock;
        bool m_generateStructures;
        bool m_hardcore;
        bool m_pvp;
        bool m_spawnAnimals;
        bool m_spawnMonsters;
        bool m_spawnNPCs;
        //Bedrock
        bool m_allowCheats;
        bool m_clientSideChunkGenerationEnabled;
        bool m_disableCustomSkins;
        unsigned int m_tickDistance;
        //Forge
    };
}

#endif //SERVERPROPERTIES_H