#include "models/serverproperties.h"
#include <sstream>

#define DEFAULT_DIFFICULTY Difficulty::Easy
#define DEFAULT_FORCE_GAMEMODE false
#define DEFAULT_GAMEMODE Gamemode::Survival
#define DEFAULT_LEVEL_NAME "world"
#define DEFAULT_LEVEL_SEED ""
#define DEFAULT_MAX_PLAYERS 10u
#define DEFAULT_ONLINE_MODE true
#define DEFAULT_PLAYER_IDLE_TIMEOUT 0u
#define DEFAULT_JAVA_SERVER_PORT 19132u
#define DEFAULT_BEDROCK_SERVER_PORT 25565u
#define DEFAULT_JAVA_VIEW_DISTANCE 10u
#define DEFAULT_BEDROCK_VIEW_DISTANCE 32u

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
        m_serverPort{ edition == Edition::Bedrock ? DEFAULT_BEDROCK_SERVER_PORT : DEFAULT_JAVA_SERVER_PORT },
        m_viewDistance{ edition == Edition::Bedrock ? DEFAULT_BEDROCK_VIEW_DISTANCE : DEFAULT_JAVA_VIEW_DISTANCE}
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
        if(maxPlayers < 0 || maxPlayers > 256)
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
        return builder.str();
    }
}