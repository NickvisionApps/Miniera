#include "models/javaserverproperties.h"
#include <sstream>

#define DEFAULT_ALLOW_FLIGHT false
#define DEFAULT_ALLOW_NETHER true
#define DEFAULT_ENABLE_COMMAND_BLOCK false
#define DEFAULT_GENERATE_STRUCTURES true
#define DEFAULT_HARDCORE false
#define DEFAULT_PVP true
#define DEFAULT_SPAWN_ANIMALS true
#define DEFAULT_SPAWN_MONSTERS true
#define DEFAULT_SPAWN_NPCS true

namespace Nickvision::Miniera::Shared::Models
{
    JavaServerProperties::JavaServerProperties(Edition edition)
        : ServerProperties{ edition == Edition::Forge ? Edition::Forge : Edition::Java },
        m_allowFlight{ DEFAULT_ALLOW_FLIGHT },
        m_allowNether{ DEFAULT_ALLOW_NETHER },
        m_enableCommandBlock{ DEFAULT_ENABLE_COMMAND_BLOCK },
        m_generateStructures{ DEFAULT_GENERATE_STRUCTURES },
        m_hardcore{ DEFAULT_HARDCORE },
        m_pvp{ DEFAULT_PVP },
        m_spawnAnimals{ DEFAULT_SPAWN_ANIMALS },
        m_spawnMonsters{ DEFAULT_SPAWN_MONSTERS },
        m_spawnNPCs{ DEFAULT_SPAWN_NPCS }
    {
        
    }

    bool JavaServerProperties::getAllowFlight() const
    {
        return m_allowFlight;
    }

    void JavaServerProperties::setAllowFlight(bool allowFlight)
    {
        m_allowFlight = allowFlight;
    }

    bool JavaServerProperties::getAllowNether() const
    {
        return m_allowNether;
    }

    void JavaServerProperties::setAllowNether(bool allowNether)
    {
        m_allowNether = allowNether;
    }

    bool JavaServerProperties::getEnableCommandBlock() const
    {
        return m_enableCommandBlock;
    }

    void JavaServerProperties::setEnableCommandBlock(bool enableCommandBlock)
    {
        m_enableCommandBlock = enableCommandBlock;
    }

    bool JavaServerProperties::getGenerateStructures() const
    {
        return m_generateStructures;
    }

    void JavaServerProperties::setGenerateStructures(bool generateStructures)
    {
        m_generateStructures = generateStructures;
    }

    bool JavaServerProperties::getHardcore() const
    {
        return m_hardcore;
    }

    void JavaServerProperties::setHardcore(bool hardcore)
    {
        m_hardcore = hardcore;
    }

    bool JavaServerProperties::getPVP() const
    {
        return m_pvp;
    }

    void JavaServerProperties::setPVP(bool pvp)
    {
        m_pvp = pvp;
    }

    bool JavaServerProperties::getSpawnAnimals() const
    {
        return m_spawnAnimals;
    }

    void JavaServerProperties::setSpawnAnimals(bool spawnAnimals)
    {
        m_spawnAnimals = spawnAnimals;
    }

    bool JavaServerProperties::getSpawnMonsters() const
    {
        return m_spawnMonsters;
    }

    void JavaServerProperties::setSpawnMonsters(bool spawnMonsters)
    {
        m_spawnMonsters = spawnMonsters;
    }

    bool JavaServerProperties::getSpawnNPCs() const
    {
        return m_spawnNPCs;
    }

    void JavaServerProperties::setSpawnNPCs(bool spawnNPCs)
    {
        m_spawnNPCs = spawnNPCs;
    }

    std::string JavaServerProperties::toString() const
    {
        std::stringstream builder;
        builder << ServerProperties::toString();
        builder << "allow-flight=" << (m_allowFlight ? "true" : "false") << std::endl;
        builder << "allow-nether=" << (m_allowNether ? "true" : "false") << std::endl;
        builder << "enable-command-block=" << (m_enableCommandBlock ? "true" : "false") << std::endl;
        builder << "generate-structures=" << (m_generateStructures ? "true" : "false") << std::endl;
        builder << "hardcore=" << (m_hardcore ? "true" : "false") << std::endl;
        builder << "pvp=" << (m_pvp ? "true" : "false") << std::endl;
        builder << "spawn-animals=" << (m_spawnAnimals ? "true" : "false") << std::endl;
        builder << "spawn-monsters=" << (m_spawnMonsters ? "true" : "false") << std::endl;
        builder << "spawn-npcs=" << (m_spawnNPCs ? "true" : "false") << std::endl;
        return builder.str();
    }
}