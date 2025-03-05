#include "models/server.h"
#include <format>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <thread>
#include <libnick/helpers/codehelpers.h>
#include <libnick/localization/gettext.h>
#include <libnick/network/web.h>
#include "models/zipfile.h"

#define CONTINUE_DOWNLOAD 0
#define JAVA_SERVER_FILE m_serverDirectory / "server.jar"
#define BEDROCK_SERVER_FILE m_serverDirectory / "server.zip"
#define FORGE_SERVER_FILE m_serverDirectory / "installer.jar"

using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Network;

namespace Nickvision::Miniera::Shared::Models
{
    Server::Server(const ServerVersion& serverVersion, const ServerProperties& serverProperties, const std::filesystem::path& serverDirectory)
        : m_serverVersion{ serverVersion },
        m_serverProperties{ serverProperties },
        m_serverDirectory{ serverDirectory },
        m_isInitialized{ false }
    {
        std::filesystem::create_directories(m_serverDirectory);
        writeJsonToDisk();
    }

    Server::Server(boost::json::object json)
        : m_serverVersion{ json["Version"].is_object() ? json["Version"].as_object() : boost::json::object() },
        m_serverProperties{ json["Properties"].is_object() ? json["Properties"].as_object() : boost::json::object() },
        m_serverDirectory{ json["Directory"].is_string() ? json["Directory"].as_string().c_str() : "" },
        m_isInitialized{ json["IsInitialized"].is_bool() ? json["IsInitialized"].as_bool() : false }
    {
        if(!std::filesystem::exists(m_serverDirectory))
        {
            throw std::invalid_argument("Invalid JSON object");
        }
    }

    Event<ServerInitializationProgressChangedEventArgs>& Server::initializationProgressChanged()
    {
        return m_initializationProgressChanged;
    }

    const std::string& Server::getName() const
    {
        return m_serverProperties.getLevelName();
    }

    const ServerVersion& Server::getVersion() const
    {
        return m_serverVersion;
    }

    bool Server::isInitialized() const
    {
        return m_isInitialized;
    }

    void Server::initialize()
    {
        if(m_isInitialized)
        {
            m_initializationProgressChanged.invoke({ getName(), 1.0, _("[Initialization] Server already initialized."), true, false });
            return;
        }
        std::thread worker{ [this]()
        {
            std::string log{ _("[Initialization] Starting server initialization process...") };
            //Donwload server files
            bool downloadSuccessful{ false };
            std::function<int(curl_off_t, curl_off_t, curl_off_t, curl_off_t)> downloadProgress{ [this, &log](curl_off_t dltotal, curl_off_t dlnow, curl_off_t, curl_off_t)
            {
                log += std::vformat(_("[Download] {}% completed"), std::make_format_args(CodeHelpers::unmove((dlnow / dltotal) * 100)));
                m_initializationProgressChanged.invoke({ getName(), static_cast<double>(dlnow) / static_cast<double>(dltotal), log, false, false });
                return CONTINUE_DOWNLOAD;
            } };
            log += _("[Download] Starting server files download...");
            m_initializationProgressChanged.invoke({ getName(), 0.0, log, false, false });
            if(m_serverVersion.getEdition() == Edition::Java)
            {
                boost::json::value json = Web::fetchJson(m_serverVersion.getReleaseUrl());
                if(json.is_object())
                {
                    downloadSuccessful = Web::downloadFile(json.as_object()["downloads"].as_object()["server"].as_object()["url"].as_string().c_str(), JAVA_SERVER_FILE, downloadProgress);
                }
            }
            else if(m_serverVersion.getEdition() == Edition::Bedrock)
            {
                downloadSuccessful = Web::downloadFile(m_serverVersion.getReleaseUrl(), BEDROCK_SERVER_FILE, downloadProgress);
            }
            else if(m_serverVersion.getEdition() == Edition::Forge)
            {
                downloadSuccessful = Web::downloadFile(m_serverVersion.getReleaseUrl(), FORGE_SERVER_FILE, downloadProgress);
            }
            if(!downloadSuccessful)
            {
                log += _("[Download] Error downloading server files.");
                m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, true });
                return;
            }
            log += _("[Download] Server files download completed.");
            //Extract server files
            bool extractionSuccessful{ false };
            log += _("[Extract] Starting server files extraction...");
            m_initializationProgressChanged.invoke({ getName(), 0.0, log, false, false });
            if(m_serverVersion.getEdition() == Edition::Java)
            {

            }
            else if(m_serverVersion.getEdition() == Edition::Bedrock)
            {

            }
            else if(m_serverVersion.getEdition() == Edition::Forge)
            {

            }
            if(!extractionSuccessful)
            {
                log += _("[Extract] Error extracting server files.");
                m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, true });
                return;
            }
            log += _("[Extract] Server files extraction completed.");
            //Write eula file
            //Write server properties file
            m_isInitialized = true;
            writeJsonToDisk();
            log += _("[Initialization] Server initialization process completed..");
            m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, false });
        } };
        worker.detach();
    }

    boost::json::object Server::toJson() const
    {
        boost::json::object json;
        json["Version"] = m_serverVersion.toJson();
        json["Properties"] = m_serverProperties.toJson();
        json["Directory"] = m_serverDirectory.string();
        json["IsInitialized"] = m_isInitialized;
        return json;
    }

    void Server::writeJsonToDisk() const
    {
        std::ofstream file{ m_serverDirectory / "miniera.json" };
        file << toJson() << std::endl;
    }
}
