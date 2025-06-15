#include "models/serverproperties.h"
#include <sstream>

#define DEFAULT_DIFFICULTY Difficulty::Easy
#define DEFAULT_FORCE_GAMEMODE false
#define DEFAULT_GAMEMODE Gamemode::Survival
#define DEFAULT_LEVEL_NAME "World"
#define DEFAULT_LEVEL_SEED ""
#define DEFAULT_MAX_PLAYERS 10u
#define DEFAULT_ONLINE_MODE true
#define DEFAULT_PLAYER_IDLE_TIMEOUT 0u
#define DEFAULT_SERVER_PORT 19132u
#define DEFAULT_JAVA_VIEW_DISTANCE 10u
#define DEFAULT_BEDROCK_VIEW_DISTANCE 32u
#define DEFAULT_ALLOW_FLIGHT false
#define DEFAULT_ALLOW_NETHER true
#define DEFAULT_ENABLE_COMMAND_BLOCK false
#define DEFAULT_GENERATE_STRUCTURES true
#define DEFAULT_HARDCORE false
#define DEFAULT_PVP true
#define DEFAULT_SPAWN_ANIMALS true
#define DEFAULT_SPAWN_MONSTERS true
#define DEFAULT_SPAWN_NPCS true
#define DEFAULT_ALLOW_CHEATS false
#define DEFAULT_CLIENT_SIDE_CHUNK_GENERATION_ENABLED true
#define DEFAULT_DISABLE_CUSTOM_SKINS false
#define DEFAULT_TICK_DISTANCE 4

namespace Nickvision::Miniera::Shared::Models
{
    ServerProperties::ServerProperties(Edition edition)
        : m_edition{ edition },
        m_difficulty{ DEFAULT_DIFFICULTY },
        m_forceGamemode{ DEFAULT_FORCE_GAMEMODE },
        m_gamemode{ DEFAULT_GAMEMODE },
        m_levelName{ DEFAULT_LEVEL_NAME },
        m_levelSeed{ DEFAULT_LEVEL_SEED },
        m_maxPlayers{ DEFAULT_MAX_PLAYERS },
        m_onlineMode{ DEFAULT_ONLINE_MODE },
        m_playerIdleTimeout{ DEFAULT_PLAYER_IDLE_TIMEOUT },
        m_serverPort{ DEFAULT_SERVER_PORT },
        m_viewDistance{ edition == Edition::Bedrock ? DEFAULT_BEDROCK_VIEW_DISTANCE : DEFAULT_JAVA_VIEW_DISTANCE},
        m_allowFlight{ DEFAULT_ALLOW_FLIGHT },
        m_allowNether{ DEFAULT_ALLOW_NETHER },
        m_enableCommandBlock{ DEFAULT_ENABLE_COMMAND_BLOCK },
        m_generateStructures{ DEFAULT_GENERATE_STRUCTURES },
        m_hardcore{ DEFAULT_HARDCORE },
        m_pvp{ DEFAULT_PVP },
        m_spawnAnimals{ DEFAULT_SPAWN_ANIMALS },
        m_spawnMonsters{ DEFAULT_SPAWN_MONSTERS },
        m_spawnNPCs{ DEFAULT_SPAWN_NPCS },
        m_allowCheats{ DEFAULT_ALLOW_CHEATS },
        m_clientSideChunkGenerationEnabled{ DEFAULT_CLIENT_SIDE_CHUNK_GENERATION_ENABLED },
        m_disableCustomSkins{ DEFAULT_DISABLE_CUSTOM_SKINS },
        m_tickDistance{ DEFAULT_TICK_DISTANCE }
    {

    }

    ServerProperties::ServerProperties(boost::json::object json)
        : m_edition{ json["Edition"].is_int64() ? static_cast<Edition>(json["Edition"].as_int64()) : Edition::Java },
        m_difficulty{ json["Difficulty"].is_int64() ? static_cast<Difficulty>(json["Difficulty"].as_int64()) : DEFAULT_DIFFICULTY },
        m_forceGamemode{ json["ForceGamemode"].is_bool() ? json["ForceGamemode"].as_bool() : DEFAULT_FORCE_GAMEMODE },
        m_gamemode{ json["Gamemode"].is_int64() ? static_cast<Gamemode>(json["Gamemode"].as_int64()) : DEFAULT_GAMEMODE },
        m_levelName{ json["LevelName"].is_string() ? json["LevelName"].as_string().c_str() : DEFAULT_LEVEL_NAME },
        m_levelSeed{ json["LevelSeed"].is_string() ? json["LevelSeed"].as_string().c_str() : DEFAULT_LEVEL_SEED },
        m_maxPlayers{ json["MaxPlayers"].is_uint64() ? static_cast<unsigned int>(json["MaxPlayers"].as_uint64()) : DEFAULT_MAX_PLAYERS },
        m_onlineMode{ json["OnlineMode"].is_bool() ? json["OnlineMode"].as_bool() : DEFAULT_ONLINE_MODE },
        m_playerIdleTimeout{ json["PlayerIdleTimeout"].is_uint64() ? static_cast<unsigned int>(json["PlayerIdleTimeout"].as_uint64()) : DEFAULT_PLAYER_IDLE_TIMEOUT },
        m_serverPort{ json["ServerPort"].is_uint64() ? static_cast<unsigned int>(json["ServerPort"].as_uint64()) : DEFAULT_SERVER_PORT },
        m_viewDistance{ json["ViewDistance"].is_uint64() ? static_cast<unsigned int>(json["ViewDistance"].as_uint64()) : DEFAULT_JAVA_VIEW_DISTANCE },
        m_allowFlight{ json["AllowFlight"].is_bool() ? json["AllowFlight"].as_bool() : DEFAULT_ALLOW_FLIGHT },
        m_allowNether{ json["AllowNether"].is_bool() ? json["AllowNether"].as_bool() : DEFAULT_ALLOW_NETHER },
        m_enableCommandBlock{ json["EnableCommandBlock"].is_bool() ? json["EnableCommandBlock"].as_bool() : DEFAULT_ENABLE_COMMAND_BLOCK },
        m_generateStructures{ json["GenerateStructures"].is_bool() ? json["GenerateStructures"].as_bool() : DEFAULT_GENERATE_STRUCTURES },
        m_hardcore{ json["Hardcore"].is_bool() ? json["Hardcore"].as_bool() : DEFAULT_HARDCORE },
        m_pvp{ json["PVP"].is_bool() ? json["PVP"].as_bool() : DEFAULT_PVP },
        m_spawnAnimals{ json["SpawnAnimals"].is_bool() ? json["SpawnAnimals"].as_bool() : DEFAULT_SPAWN_ANIMALS },
        m_spawnMonsters{ json["SpawnMonsters"].is_bool() ? json["SpawnMonsters"].as_bool() : DEFAULT_SPAWN_MONSTERS },
        m_spawnNPCs{ json["SpawnNPCs"].is_bool() ? json["SpawnNPCs"].as_bool() : DEFAULT_SPAWN_NPCS },
        m_allowCheats{ json["AllowCheats"].is_bool() ? json["AllowCheats"].as_bool() : DEFAULT_ALLOW_CHEATS },
        m_clientSideChunkGenerationEnabled{ json["ClientSideChunkGenerationEnabled"].is_bool() ? json["ClientSideChunkGenerationEnabled"].as_bool() : DEFAULT_CLIENT_SIDE_CHUNK_GENERATION_ENABLED },
        m_disableCustomSkins{ json["DisableCustomSkins"].is_bool() ? json["DisableCustomSkins"].as_bool() : DEFAULT_DISABLE_CUSTOM_SKINS },
        m_tickDistance{ json["TickDistance"].is_uint64() ? static_cast<unsigned int>(json["TickDistance"].as_uint64()) : DEFAULT_TICK_DISTANCE }
    {

    }

    Difficulty ServerProperties::getDifficulty() const
    {
        return m_difficulty;
    }

    void ServerProperties::setDifficulty(Difficulty difficulty)
    {
        m_difficulty = difficulty;
    }

    bool ServerProperties::getForceGamemode() const
    {
        return m_forceGamemode;
    }

    void ServerProperties::setForceGamemode(bool forceGamemode)
    {
        m_forceGamemode = forceGamemode;
    }

    Gamemode ServerProperties::getGamemode() const
    {
        return m_gamemode;
    }

    void ServerProperties::setGamemode(Gamemode gamemode)
    {
        m_gamemode = gamemode;
    }

    const std::string& ServerProperties::getLevelName() const
    {
        return m_levelName;
    }

    void ServerProperties::setLevelName(const std::string& levelName)
    {
        if(levelName.empty())
        {
            m_levelName = DEFAULT_LEVEL_NAME;
        }
        else 
        {
            m_levelName = levelName;
        }
    }

    const std::string& ServerProperties::getLevelSeed() const
    {
        return m_levelSeed;
    }

    void ServerProperties::setLevelSeed(const std::string& levelSeed)
    {
        m_levelSeed = levelSeed;
    }

    unsigned int ServerProperties::getMaxPlayers() const
    {
        return m_maxPlayers;
    }

    void ServerProperties::setMaxPlayers(unsigned int maxPlayers)
    {
        if(maxPlayers < 1 || maxPlayers > 256)
        {
            maxPlayers = DEFAULT_MAX_PLAYERS;
        }
        m_maxPlayers = maxPlayers;
    }

    bool ServerProperties::getOnlineMode() const
    {
        return m_onlineMode;
    }

    void ServerProperties::setOnlineMode(bool onlineMode)
    {
        m_onlineMode = onlineMode;
    }

    unsigned int ServerProperties::getPlayerIdleTimeout() const
    {
        return m_playerIdleTimeout;
    }

    void ServerProperties::setPlayerIdleTimeout(unsigned int playerIdleTimeout)
    {
        m_playerIdleTimeout = playerIdleTimeout;
    }

    unsigned int ServerProperties::getServerPort() const
    {
        return m_serverPort;
    }

    void ServerProperties::setServerPort(unsigned int serverPort)
    {
        m_serverPort = serverPort;
    }

    unsigned int ServerProperties::getViewDistance() const
    {
        return m_viewDistance;
    }

    void ServerProperties::setViewDistance(unsigned int viewDistance)
    {
        if(m_edition == Edition::Bedrock)
        {
            if(viewDistance < 5)
            {
                viewDistance = DEFAULT_BEDROCK_VIEW_DISTANCE;
            }
        }
        else
        {
            if(viewDistance < 3 || viewDistance > 32)
            {
                viewDistance = DEFAULT_JAVA_VIEW_DISTANCE;
            }
        }
        m_viewDistance = viewDistance;
    }

    bool ServerProperties::getAllowFlight() const
    {
        return m_allowFlight;
    }

    void ServerProperties::setAllowFlight(bool allowFlight)
    {
        m_allowFlight = allowFlight;
    }

    bool ServerProperties::getAllowNether() const
    {
        return m_allowNether;
    }

    void ServerProperties::setAllowNether(bool allowNether)
    {
        m_allowNether = allowNether;
    }

    bool ServerProperties::getEnableCommandBlock() const
    {
        return m_enableCommandBlock;
    }

    void ServerProperties::setEnableCommandBlock(bool enableCommandBlock)
    {
        m_enableCommandBlock = enableCommandBlock;
    }

    bool ServerProperties::getGenerateStructures() const
    {
        return m_generateStructures;
    }

    void ServerProperties::setGenerateStructures(bool generateStructures)
    {
        m_generateStructures = generateStructures;
    }

    bool ServerProperties::getHardcore() const
    {
        return m_hardcore;
    }

    void ServerProperties::setHardcore(bool hardcore)
    {
        m_hardcore = hardcore;
    }

    bool ServerProperties::getPVP() const
    {
        return m_pvp;
    }

    void ServerProperties::setPVP(bool pvp)
    {
        m_pvp = pvp;
    }

    bool ServerProperties::getSpawnAnimals() const
    {
        return m_spawnAnimals;
    }

    void ServerProperties::setSpawnAnimals(bool spawnAnimals)
    {
        m_spawnAnimals = spawnAnimals;
    }

    bool ServerProperties::getSpawnMonsters() const
    {
        return m_spawnMonsters;
    }

    void ServerProperties::setSpawnMonsters(bool spawnMonsters)
    {
        m_spawnMonsters = spawnMonsters;
    }

    bool ServerProperties::getSpawnNPCs() const
    {
        return m_spawnNPCs;
    }

    void ServerProperties::setSpawnNPCs(bool spawnNPCs)
    {
        m_spawnNPCs = spawnNPCs;
    }

    bool ServerProperties::getAllowCheats() const
    {
        return m_allowCheats;
    }

    void ServerProperties::setAllowCheats(bool allowCheats)
    {
        m_allowCheats = allowCheats;
    }

    bool ServerProperties::getClientSideChunkGenerationEnabled() const
    {
        return m_clientSideChunkGenerationEnabled;
    }

    void ServerProperties::setClientSideChunkGenerationEnabled(bool clientSideChunkGenerationEnabled)
    {
        m_clientSideChunkGenerationEnabled = clientSideChunkGenerationEnabled;
    }

    bool ServerProperties::getDisableCustomSkins() const
    {
        return m_disableCustomSkins;
    }

    void ServerProperties::setDisableCustomSkins(bool disableCustomSkins)
    {
        m_disableCustomSkins = disableCustomSkins;
    }

    unsigned int ServerProperties::getTickDistance() const
    {
        return m_tickDistance;
    }

    void ServerProperties::setTickDistance(unsigned int tickDistance)
    {
        if(tickDistance < 4 || tickDistance > 12)
        {
            tickDistance = DEFAULT_TICK_DISTANCE;
        }
        m_tickDistance = tickDistance;
    }

    std::string ServerProperties::toString() const
    {
        std::stringstream builder;
        builder << "difficulty=" << static_cast<int>(m_difficulty) << '\n';
        builder << "force-gamemode=" << (m_forceGamemode ? "true" : "false") << '\n';
        builder << "gamemode=" << static_cast<int>(m_gamemode) << '\n';
        builder << "level-name=" << m_levelName << '\n';
        builder << "level-seed=" << m_levelSeed << '\n';
        builder << "max-players=" << m_maxPlayers << '\n';
        builder << "online-mode=" << (m_onlineMode ? "true" : "false") << '\n';
        builder << "player-idle-timeout=" << m_playerIdleTimeout << '\n';
        builder << "server-port=" << m_serverPort << '\n';
        builder << "view-distance=" << m_viewDistance << '\n';
        if(m_edition == Edition::Bedrock)
        {
            builder << "allow-cheats=" << m_allowCheats << "\n";
            builder << "client-side-chunk-generation-enabled=" << m_clientSideChunkGenerationEnabled << "\n";
            builder << "disable-custom-skins=" << m_disableCustomSkins << "\n";
            builder << "tick-distance=" << m_tickDistance << "\n";
        }
        else
        {
            builder << "allow-flight=" << (m_allowFlight ? "true" : "false") << std::endl;
            builder << "allow-nether=" << (m_allowNether ? "true" : "false") << std::endl;
            builder << "enable-command-block=" << (m_enableCommandBlock ? "true" : "false") << std::endl;
            builder << "generate-structures=" << (m_generateStructures ? "true" : "false") << std::endl;
            builder << "hardcore=" << (m_hardcore ? "true" : "false") << std::endl;
            builder << "pvp=" << (m_pvp ? "true" : "false") << std::endl;
            builder << "spawn-animals=" << (m_spawnAnimals ? "true" : "false") << std::endl;
            builder << "spawn-monsters=" << (m_spawnMonsters ? "true" : "false") << std::endl;
            builder << "spawn-npcs=" << (m_spawnNPCs ? "true" : "false") << std::endl;
        }
        return builder.str();
    }

    boost::json::object ServerProperties::toJson() const
    {
        boost::json::object json;
        json["Edition"] = static_cast<int>(m_edition);
        json["Difficulty"] = static_cast<int>(m_difficulty);
        json["ForceGamemode"] = m_forceGamemode;
        json["Gamemode"] = static_cast<int>(m_gamemode);
        json["LevelName"] = m_levelName;
        json["LevelSeed"] = m_levelSeed;
        json["MaxPlayers"] = m_maxPlayers;
        json["OnlineMode"] = m_onlineMode;
        json["PlayerIdleTimeout"] = m_playerIdleTimeout;
        json["ServerPort"] = m_serverPort;
        json["ViewDistance"] = m_viewDistance;
        if(m_edition == Edition::Bedrock)
        {
            json["AllowCheats"] = m_allowCheats;
            json["ClientSideChunkGenerationEnabled"] = m_clientSideChunkGenerationEnabled;
            json["DisableCustomSkins"] = m_disableCustomSkins;
            json["TickDistance"] = m_tickDistance;
        }
        else
        {
            json["AllowFlight"] = m_allowFlight;
            json["AllowNether"] = m_allowNether;
            json["EnableCommandBlock"] = m_enableCommandBlock;
            json["GenerateStructures"] = m_generateStructures;
            json["Hardcore"] = m_hardcore;
            json["PVP"] = m_pvp;
            json["SpawnAnimals"] = m_spawnAnimals;
            json["SpawnMonsters"] = m_spawnMonsters;
            json["SpawnNPCs"] = m_spawnNPCs;
        }
        return json;
    }
}
