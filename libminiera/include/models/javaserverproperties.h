#ifndef JAVASERVERPROPERTIES_H
#define JAVASERVERPROPERTIES_H

#include <string>
#include "serverproperties.h"

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of properties for a Java Minecraft server.
     */
    class JavaServerProperties : public ServerProperties
    {
    public:
        /**
         * @brief Constructs a JavaServerProperties.
         * @param edition The edition of the server
         */
        JavaServerProperties(Edition edition = Edition::Java);
        /**
         * @brief Gets whether or not flight is allowed.
         * @return Whether or not flight is allowed
         */
        bool getAllowFlight() const;
        /**
         * @brief Sets whether or not flight is allowed.
         * @param allowFlight Whether or not flight is allowed
         */
        void setAllowFlight(bool allowFlight);
        /**
         * @brief Gets whether or not the Nether is allowed.
         * @return Whether or not the Nether is allowed
         */
        bool getAllowNether() const;
        /**
         * @brief Sets whether or not the Nether is allowed.
         * @param allowNether Whether or not the Nether is allowed
         */
        void setAllowNether(bool allowNether);
        /**
         * @brief Gets whether or not command blocks are enabled.
         * @return Whether or not command blocks are enabled
         */ 
        bool getEnableCommandBlock() const;
        /**
         * @brief Sets whether or not command blocks are enabled.
         * @param enableCommandBlock Whether or not command blocks are enabled
         */
        void setEnableCommandBlock(bool enableCommandBlock);
        /**
         * @brief Gets whether or not structures are generated.
         * @return Whether or not structures are generated
         */
        bool getGenerateStructures() const;
        /**
         * @brief Sets whether or not structures are generated.
         * @param generateStructures Whether or not structures are generated
         */
        void setGenerateStructures(bool generateStructures);
        /**
         * @brief Gets whether or not hardcore mode is enabled.
         * @return Whether or not hardcore mode is enabled
         */
        bool getHardcore() const;
        /**
         * @brief Sets whether or not hardcore mode is enabled.
         * @param hardcore Whether or not hardcore mode is enabled
         */
        void setHardcore(bool hardcore);
        /**
         * @brief Gets whether or not PVP is enabled.
         * @return Whether or not PVP is enabled
         */
        bool getPVP() const;
        /**
         * @brief Sets whether or not PVP is enabled.
         * @param pvp Whether or not PVP is enabled
         */
        void setPVP(bool pvp);
        /**
         * @brief Gets whether or not animals spawn.
         * @return Whether or not animals spawn
         */
        bool getSpawnAnimals() const;
        /**
         * @brief Sets whether or not animals spawn.
         * @param spawnAnimals Whether or not animals spawn
         */
        void setSpawnAnimals(bool spawnAnimals);
        /**
         * @brief Gets whether or not monsters spawn.
         * @return Whether or not monsters spawn
         */
        bool getSpawnMonsters() const;
        /**
         * @brief Sets whether or not monsters spawn.
         * @param spawnMonsters Whether or not monsters spawn
         */
        void setSpawnMonsters(bool spawnMonsters);
        /**
         * @brief Gets whether or not NPCs spawn.
         * @return Whether or not NPCs spawn
         */
        bool getSpawnNPCs() const;
        /**
         * @brief Sets whether or not NPCs spawn.
         * @param spawnNPCs Whether or not NPCs spawn
         */
        void setSpawnNPCs(bool spawnNPCs);
        /**
         * @brief Converts the server properties to a string to save to a file.
         * @return The server properties as a string
         */
        virtual std::string toString() const override;

    protected:
        bool m_allowFlight;
        bool m_allowNether;
        bool m_enableCommandBlock;
        bool m_generateStructures;
        bool m_hardcore;
        bool m_pvp;
        bool m_spawnAnimals;
        bool m_spawnMonsters;
        bool m_spawnNPCs;
    };
}

#endif //JAVASERVERPROPERTIES_H