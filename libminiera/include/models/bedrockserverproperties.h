#ifndef BEDROCKSERVERPROPERTIES_H
#define BEDROCKSERVERPROPERTIES_H

#include <string>
#include "serverproperties.h"

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of properties for a Bedrock Minecraft server.
     */
    class BedrockServerProperties : public ServerProperties
    {
    public:
        /**
         * @brief Constructs a BedrockServerProperties.
         */
        BedrockServerProperties();
        /**
         * @brief Gets whether or not cheats are allowed.
         * @return Whether or not cheats are allowed
         */
        bool getAllowCheats() const;
        /**
         * @brief Sets whether or not cheats are allowed.
         * @param allowCheats Whether or not cheats are allowed
         */
        void setAllowCheats(bool allowCheats);
        /**
         * @brief Gets whether or not client-side chunk generation is enabled.
         * @return Whether or not client-side chunk generation is enabled
         */
        bool getClientSideChunkGenerationEnabled() const;
        /**
         * @brief Sets whether or not client-side chunk generation is enabled.
         * @param clientSideChunkGenerationEnabled Whether or not client-side chunk generation is enabled
         */
        void setClientSideChunkGenerationEnabled(bool clientSideChunkGenerationEnabled);
        /**
         * @brief Gets whether or not custom skins are disabled.
         * @return Whether or not custom skins are
         */
        bool getDisableCustomSkins() const;
        /**
         * @brief Sets whether or not custom skins are disabled.
         * @param disableCustomSkins Whether or not custom skins are disabled
         */
        void setDisableCustomSkins(bool disableCustomSkins);
        /**
         * @brief Gets the tick distance.
         * @return The tick distance
         */
        unsigned int getTickDistance() const;
        /**
         * @brief Sets the tick distance.
         * @param tickDistance The tick distance
         */
        void setTickDistance(unsigned int tickDistance);
        /**
         * @brief Converts the server properties to a string to save to a file.
         * @return The server properties as a string
         */
        std::string toString() const override;

    private:
        bool m_allowCheats;
        bool m_clientSideChunkGenerationEnabled;
        bool m_disableCustomSkins;
        unsigned int m_tickDistance;
    };
}

#endif //BEDROCKSERVERPROPERTIES_H