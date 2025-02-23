#include "models/bedrockserverproperties.h"
#include <sstream>

#define DEFAULT_ALLOW_CHEATS false
#define DEFAULT_CLIENT_SIDE_CHUNK_GENERATION_ENABLED true
#define DEFAULT_DISABLE_CUSTOM_SKINS false
#define DEFAULT_TICK_DISTANCE 4

namespace Nickvision::Miniera::Shared::Models
{
    BedrockServerProperties::BedrockServerProperties()
        : ServerProperties{ Edition::Bedrock },
        m_allowCheats{ DEFAULT_ALLOW_CHEATS },
        m_clientSideChunkGenerationEnabled{ DEFAULT_CLIENT_SIDE_CHUNK_GENERATION_ENABLED },
        m_disableCustomSkins{ DEFAULT_DISABLE_CUSTOM_SKINS },
        m_tickDistance{ DEFAULT_TICK_DISTANCE }
    {

    }

    bool BedrockServerProperties::getAllowCheats() const
    {
        return m_allowCheats;
    }

    void BedrockServerProperties::setAllowCheats(bool allowCheats)
    {
        m_allowCheats = allowCheats;
    }

    bool BedrockServerProperties::getClientSideChunkGenerationEnabled() const
    {
        return m_clientSideChunkGenerationEnabled;
    }

    void BedrockServerProperties::setClientSideChunkGenerationEnabled(bool clientSideChunkGenerationEnabled)
    {
        m_clientSideChunkGenerationEnabled = clientSideChunkGenerationEnabled;
    }

    bool BedrockServerProperties::getDisableCustomSkins() const
    {
        return m_disableCustomSkins;
    }

    void BedrockServerProperties::setDisableCustomSkins(bool disableCustomSkins)
    {
        m_disableCustomSkins = disableCustomSkins;
    }

    unsigned int BedrockServerProperties::getTickDistance() const
    {
        return m_tickDistance;
    }

    void BedrockServerProperties::setTickDistance(unsigned int tickDistance)
    {
        if(tickDistance < 4 || tickDistance > 12)
        {
            tickDistance = DEFAULT_TICK_DISTANCE;
        }
        m_tickDistance = tickDistance;
    }

    std::string BedrockServerProperties::toString() const
    {
        std::stringstream builder;
        builder << ServerProperties::toString();
        builder << "allow-cheats=" << m_allowCheats << "\n";
        builder << "client-side-chunk-generation-enabled=" << m_clientSideChunkGenerationEnabled << "\n";
        builder << "disable-custom-skins=" << m_disableCustomSkins << "\n";
        builder << "tick-distance=" << m_tickDistance << "\n";
        return builder.str();
    }
}