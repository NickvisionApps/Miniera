#include "models/server.h"
#include <fstream>
#include <stdexcept>
#include <functional>
#include <thread>
#include <libnick/helpers/codehelpers.h>
#include <libnick/localization/gettext.h>
#include <libnick/network/web.h>
#include <libnick/system/environment.h>
#include <libnick/system/process.h>
#include "models/zipfile.h"

#define SERVER_PROPERTIES_FILE (m_serverDirectory / "server.properties")
#define EULA_FILE (m_serverDirectory / "eula.txt")
#define JAVA_SERVER_FILE_RAW (m_serverDirectory / "server.jar")
#define JAVA_SERVER_FILE_EXTRACTED (m_serverDirectory / "server.jar")
#define BEDROCK_SERVER_FILE_RAW (m_serverDirectory / "server.zip")
#ifdef _WIN32
#define BEDROCK_SERVER_FILE_EXTRACTED (m_serverDirectory / "bedrock_server.exe")
#else
#define BEDROCK_SERVER_FILE_EXTRACTED (m_serverDirectory / "bedrock_server")
#endif
#define FORGE_SERVER_FILE_RAW (m_serverDirectory / "installer.jar")
#ifdef _WIN32
#define FORGE_SERVER_FILE_EXTRACTED (m_serverDirectory / "run.bat")
#else
#define FORGE_SERVER_FILE_EXTRACTED (m_serverDirectory / "run.sh")
#endif

using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Miniera::Shared::Models;
using namespace Nickvision::Network;
using namespace Nickvision::System;

namespace Nickvision::Miniera::Shared::Models
{
    Server::Server(const ServerVersion& serverVersion, const ServerProperties& serverProperties, const std::filesystem::path& serverDirectory)
        : m_serverVersion{ serverVersion },
        m_serverProperties{ serverProperties },
        m_serverDirectory{ serverDirectory }
    {
        std::filesystem::create_directories(m_serverDirectory);
        writeJsonToDisk();
    }

    Server::Server(boost::json::object json)
        : m_serverVersion{ json["Version"].is_object() ? json["Version"].as_object() : boost::json::object() },
        m_serverProperties{ json["Properties"].is_object() ? json["Properties"].as_object() : boost::json::object() },
        m_serverDirectory{ json["Directory"].is_string() ? json["Directory"].as_string().c_str() : "" }
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

    void Server::initialize()
    {
        if(initializeCheck())
        {
            m_initializationProgressChanged.invoke({ getName(), 1.0, _("[Initialization] Server already initialized."), true, false });
            return;
        }
        std::thread worker{ [this]()
        {
            std::string log{ _("[Initialization] Starting server initialization process...\n") };
            if(!initializeDownload(log))
            {
                return;
            }
            if(!initializeExtract(log))
            {
                return;
            }
            if(!initializeWrite(log))
            {
                return;
            }
            log += _("[Initialization] Server initialization process completed..\n");
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
        return json;
    }

    void Server::writeJsonToDisk() const
    {
        std::ofstream file{ m_serverDirectory / "miniera.json" };
        file << toJson() << std::endl;
    }

    bool Server::initializeCheck()
    {
        std::filesystem::create_directories(m_serverDirectory);
        if(m_serverVersion.getEdition() == Edition::Java)
        {
            return std::filesystem::exists(JAVA_SERVER_FILE_EXTRACTED) && std::filesystem::exists(EULA_FILE) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        else if(m_serverVersion.getEdition() == Edition::Bedrock)
        {
            return std::filesystem::exists(BEDROCK_SERVER_FILE_EXTRACTED) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        else if(m_serverVersion.getEdition() == Edition::Forge)
        {
            return std::filesystem::exists(FORGE_SERVER_FILE_EXTRACTED) && std::filesystem::exists(EULA_FILE) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        return false;
    }

    bool Server::initializeDownload(std::string& log)
    {
        bool downloadSuccessful{ false };
        curl_off_t oldNow{ 0 };
        curl_off_t oldTotal{ 0 };
        std::function<int(curl_off_t, curl_off_t, curl_off_t, curl_off_t)> downloadProgress{ [&](curl_off_t dltotal, curl_off_t dlnow, curl_off_t, curl_off_t)
        {
            if(oldNow != dlnow || oldTotal != dltotal)
            {
                oldNow = dlnow;
                oldTotal = dltotal;
                m_initializationProgressChanged.invoke({ getName(), static_cast<double>(oldNow) / static_cast<double>(oldTotal), log, false, false });
            }
            return 0;
        }};
        log += _("[Download] Starting server files download...\n");
        m_initializationProgressChanged.invoke({ getName(), 0.0, log, false, false });
        if(m_serverVersion.getEdition() == Edition::Java)
        {
            boost::json::value json = Web::fetchJson(m_serverVersion.getReleaseUrl());
            if(json.is_object())
            {
                downloadSuccessful = Web::downloadFile(json.as_object()["downloads"].as_object()["server"].as_object()["url"].as_string().c_str(), JAVA_SERVER_FILE_RAW, downloadProgress);
            }
        }
        else if(m_serverVersion.getEdition() == Edition::Bedrock)
        {
            downloadSuccessful = Web::downloadFile(m_serverVersion.getReleaseUrl(), BEDROCK_SERVER_FILE_RAW, downloadProgress);
        }
        else if(m_serverVersion.getEdition() == Edition::Forge)
        {
            downloadSuccessful = Web::downloadFile(m_serverVersion.getReleaseUrl(), FORGE_SERVER_FILE_RAW, downloadProgress);
        }
        if(downloadSuccessful)
        {
            log += _("[Download] Server files download completed.\n");
        }
        else
        {
            log += _("[Download] Error downloading server files.\n");
            m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, true });
        }
        return downloadSuccessful;
    }

    bool Server::initializeExtract(std::string& log)
    {
        bool extractionSuccessful{ false };
        log += _("[Extract] Starting server files extraction...\n");
        m_initializationProgressChanged.invoke({ getName(), 0.0, log, false, false });
        if(m_serverVersion.getEdition() == Edition::Java)
        {
            Process proc{ Environment::findDependency("java"), std::vector<std::string>{ "-jar", JAVA_SERVER_FILE_RAW.string(), "--initSettings" }, m_serverDirectory };
            proc.start();
            proc.waitForExit();
            extractionSuccessful = proc.getExitCode() == 0;
        }
        else if(m_serverVersion.getEdition() == Edition::Bedrock)
        {
            std::unique_ptr<ZipFile> zip{ std::make_unique<ZipFile>(BEDROCK_SERVER_FILE_RAW) };
            std::function<void(double)> extractionProgress{ [&](double progress)
            {
                m_initializationProgressChanged.invoke({ getName(), progress, log, false, false });
            }};
            extractionSuccessful = zip->extract(m_serverDirectory, extractionProgress);
            zip.reset();
            std::filesystem::remove(BEDROCK_SERVER_FILE_RAW);
            if(Environment::getOperatingSystem() == OperatingSystem::Windows)
            {
                //Prevents UWP lookback when connecting to server from client on same host
                Environment::exec("CheckNetIsolation.exe LoopbackExempt -a -p=S-1-15-2-1958404141-86561845-1752920682-3514627264-368642714-62675701-733520436");
            }
        }
        else if(m_serverVersion.getEdition() == Edition::Forge)
        {
            Process proc{ Environment::findDependency("java"), std::vector<std::string>{ "-jar", FORGE_SERVER_FILE_RAW.string(), "--installServer" }, m_serverDirectory };
            proc.start();
            proc.waitForExit();
            extractionSuccessful = proc.getExitCode() == 0;
            std::filesystem::remove(FORGE_SERVER_FILE_RAW);
        }
        if(extractionSuccessful)
        {
            log += _("[Extract] Server files extraction completed.\n");
        }
        else
        {
            log += _("[Extract] Error extracting server files.\n");
            m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, true });
        }
        return extractionSuccessful;
    }

    bool Server::initializeWrite(std::string& log)
    {
        bool writingSuccessful{ false };
        log += _("[Write] Starting server property files writing...\n");
        m_initializationProgressChanged.invoke({ getName(), 0.0, log, false, false });
        if(m_serverVersion.getEdition() == Edition::Java || m_serverVersion.getEdition() == Edition::Forge)
        {
            std::ofstream eula{ EULA_FILE, std::ios::trunc };
            std::ofstream properties{ SERVER_PROPERTIES_FILE, std::ios::trunc };
            if(eula.is_open())
            {
                eula << "eula=true" << std::endl;
                log += _("[Write] Wrote EULA file.\n");
            }
            if(properties.is_open())
            {
                properties << m_serverProperties.toString() << std::endl;
                log += _("[Write] Wrote server properties file.\n");
            }
            writingSuccessful = eula.is_open() && properties.is_open();
            eula.close();
            properties.close();
        }
        else if(m_serverVersion.getEdition() == Edition::Bedrock)
        {
            std::ofstream properties{ SERVER_PROPERTIES_FILE, std::ios::trunc };
            if(properties.is_open())
            {
                properties << m_serverProperties.toString() << std::endl;
                log += _("[Write] Wrote server properties file.\n");
            }
            writingSuccessful = properties.is_open();
            properties.close();
        }
        if(writingSuccessful)
        {
            log += _("[Write] Server propety files writing completed.\n");
        }
        else
        {
            log += _("[Write] Error writing server property files.\n");
            m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, true });
        }
        return writingSuccessful;
    }
}
