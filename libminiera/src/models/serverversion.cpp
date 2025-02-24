#include "models/serverversion.h"
#include <algorithm>
#include <stdexcept>
#include <libnick/network/web.h>
#include <libnick/system/environment.h>

using namespace Nickvision::Network;
using namespace Nickvision::System;
using namespace Nickvision::Update;

namespace Nickvision::Miniera::Shared::Models
{
    ServerVersion::ServerVersion(Edition edition, const Version& version, const std::string& releaseUrl)
        : m_edition{ edition },
        m_version{ version },
        m_releaseUrl{ releaseUrl }
    {

    }

    ServerVersion::ServerVersion(boost::json::object json)
        : m_edition{ json["Edition"].is_int64() ? static_cast<Edition>(json["Edition"].as_int64()) : Edition::Java },
        m_version{ json["Version"].is_string() ? json["Version"].as_string().c_str() : "1.0.0" },
        m_releaseUrl{ json["ReleaseUrl"].is_string() ? json["ReleaseUrl"].as_string().c_str() : "" }
    {
        if(m_releaseUrl.empty())
        {
            throw std::invalid_argument("Invalid JSON");
        }
    }

    std::vector<ServerVersion> ServerVersion::fetch(Edition edition)
    {
        std::vector<ServerVersion> versions;
        if(edition == Edition::Java)
        {
            static const std::string manifestUrl{ "https://piston-meta.mojang.com/mc/game/version_manifest_v2.json" };
            boost::json::value json = Web::fetchJson(manifestUrl);
            if(json.is_object() && json.as_object().contains("versions"))
            {
                for(const boost::json::value& val : json.as_object()["versions"].as_array())
                {
                    if(!val.is_object())
                    {
                        continue;
                    }
                    boost::json::object version = val.as_object();
                    if(version["type"].is_string() && std::string(version["type"].as_string()) != "release")
                    {
                        continue;
                    }
                    std::string versionString{ version["id"].as_string().c_str() };
                    if(std::count(versionString.begin(), versionString.end(), '.') != 2)
                    {
                        versionString += ".0";
                    }
                    versions.push_back({ Edition::Java, { versionString }, version["url"].as_string().c_str() });
                }
            }
        }
        else if(edition == Edition::Bedrock)
        {
            static std::string url;
            if(url.empty())
            {
                if(Environment::getOperatingSystem() == OperatingSystem::Windows)
                {
                    url = "https://www.minecraft.net/bedrockdedicatedserver/bin-win/bedrock-server-1.21.61.01.zip";
                }
                else if(Environment::getOperatingSystem() == OperatingSystem::Linux)
                {
                    url = "https://www.minecraft.net/bedrockdedicatedserver/bin-linux/bedrock-server-1.21.61.01.zip";
                }
                versions.push_back({ Edition::Bedrock, { "1.21.61" }, url });
            }
        }
        else if(edition == Edition::Forge)
        {
            static const std::string manifestUrl{ "https://files.minecraftforge.net/net/minecraftforge/forge/promotions_slim.json" };
            boost::json::value json = Web::fetchJson(manifestUrl);
            if(json.is_object() && json.as_object().contains("promos"))
            {
                for(const boost::json::key_value_pair& pair : json.as_object()["promos"].as_object())
                {
                    if(!pair.key().contains("latest"))
                    {
                        continue;
                    }
                    std::string versionString{ pair.key().substr(0, pair.key().find("-")) };
                    if(std::count(versionString.begin(), versionString.end(), '.') != 2)
                    {
                        versionString += ".0";
                    }
                    std::string forgeId{ std::string(pair.key().substr(0, pair.key().find("-"))) + "-" + std::string(pair.value().as_string()) };
                    versions.push_back({ Edition::Forge, { versionString }, "https://maven.minecraftforge.net/net/minecraftforge/forge/" + forgeId + "/forge-" + forgeId + "-installer.jar" });
                }
                std::reverse(versions.begin(), versions.end());
            }
        }
        return versions;
    }

    Edition ServerVersion::getEdition() const
    {
        return m_edition;
    }

    const Version& ServerVersion::getVersion() const
    {
        return m_version;
    }

    const std::string& ServerVersion::getReleaseUrl() const
    {
        return m_releaseUrl;
    }

    std::filesystem::path ServerVersion::downloadServer(std::filesystem::path path) const
    {
        if(m_edition == Edition::Java)
        {
            if(path.filename().extension() != ".jar")
            {
                path /= "server.jar";
            }
            boost::json::value json = Web::fetchJson(m_releaseUrl);
            if(json.is_object() && json.as_object().contains("downloads"))
            {
                boost::json::object downloads = json.as_object()["downloads"].as_object();
                if(downloads.contains("server"))
                {
                    if(Web::downloadFile(downloads["server"].as_object()["url"].as_string().c_str(), path))
                    {
                        return path;
                    }
                }
            }
        }
        else if(m_edition == Edition::Bedrock)
        {
            if(path.filename().extension() != ".zip")
            {
                path /= "server.zip";
            }
            if(Web::downloadFile(m_releaseUrl, path))
            {
                return path;
            }
        }
        else if(m_edition == Edition::Forge)
        {
            if(path.filename().extension() != ".jar")
            {
                path /= "installer.jar";
            }
            if(Web::downloadFile(m_releaseUrl, path))
            {
                return path;
            }
        }
        return {};
    }

    boost::json::object ServerVersion::toJson() const
    {
        boost::json::object json;
        json["Edition"] = static_cast<int>(m_edition);
        json["Version"] = m_version.str();
        json["ReleaseUrl"] = m_releaseUrl;
        return json;
    }
}