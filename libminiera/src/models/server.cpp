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

#define MINIERA_FILE (m_directory / "miniera.json")
#define SERVER_PROPERTIES_FILE (m_directory / "server.properties")
#define EULA_FILE (m_directory / "eula.txt")
#define JAVA_SERVER_FILE_RAW (m_directory / "server.jar")
#define JAVA_SERVER_FILE_EXTRACTED (m_directory / "server.jar")
#define BEDROCK_SERVER_FILE_RAW (m_directory / "server.zip")
#ifdef _WIN32
#define BEDROCK_SERVER_FILE_EXTRACTED (m_directory / "bedrock_server.exe")
#else
#define BEDROCK_SERVER_FILE_EXTRACTED (m_directory / "bedrock_server")
#endif
#define FORGE_SERVER_FILE_RAW (m_directory / "installer.jar")

using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Miniera::Shared::Models;
using namespace Nickvision::Network;
using namespace Nickvision::System;

namespace Nickvision::Miniera::Shared::Models
{
    static bool s_serverRunning{ false };

    static std::string getJavaRamString(unsigned int ramInGB)
    {
        return "-Xmx" + std::to_string(ramInGB) + "G";
    }

    static std::filesystem::path getForgeShimFile(const std::filesystem::path& dir)
    {
        for(const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dir))
        {
            if(entry.is_regular_file() && entry.path().stem().string().find("shim") != std::string::npos)
            {
                return entry.path();
            }
        }
        return {};
    }

    Server::Server(const ServerVersion& version, const ServerProperties& properties, const std::filesystem::path& directory)
        : m_version{ version },
        m_properties{ properties },
        m_directory{ directory },
        m_initialized{ false }
    {
        std::filesystem::create_directories(m_directory);
        wrtieFilesToDisk();
    }

    Server::Server(boost::json::object json)
        : m_version{ json["Version"].is_object() ? json["Version"].as_object() : boost::json::object() },
        m_properties{ json["Properties"].is_object() ? json["Properties"].as_object() : boost::json::object() },
        m_directory{ json["Directory"].is_string() ? json["Directory"].as_string().c_str() : "" },
        m_initialized{ false }
    {
        if(!std::filesystem::exists(m_directory))
        {
            throw std::invalid_argument("Invalid JSON object");
        }
        wrtieFilesToDisk();
    }

    Server::~Server()
    {
        stop();
        wrtieFilesToDisk();
    }

    bool Server::isServerRunning()
    {
        return s_serverRunning;
    }

    Event<ServerInitializationProgressChangedEventArgs>& Server::initializationProgressChanged()
    {
        return m_initializationProgressChanged;
    }

    const std::string& Server::getName() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_properties.getLevelName();
    }

    const ServerVersion& Server::getVersion() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_version;
    }

    const std::string& Server::getOutput() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_proc)
        {
            static std::string empty;
            return empty;
        }
        return m_proc->getOutput();
    }

    ServerAddress Server::getAddress() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(m_proc && m_broadcaster)
        {
            return m_broadcaster->getAddress();
        }
        return { "localhost", m_properties.getServerPort() };
    }

    double Server::getCPUUsage() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_proc)
        {
            return 0.0;
        }
        return m_proc->getCPUUsage();
    }

    unsigned long long Server::getRAMUsage() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_proc)
        {
            return 0L;
        }
        return m_proc->getRAMUsage();
    }

    bool Server::isRunning() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        return m_proc ? true : false;
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
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_initialized = true;
            lock.unlock();
            m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, false });
        } };
        worker.detach();
    }

    bool Server::start(unsigned int maxServerRamInGB)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_initialized)
        {
            return false;
        }
        if(s_serverRunning)
        {
            return m_proc ? true : false;
        }
        switch(m_version.getEdition())
        {
        case Edition::Java:
            m_proc = std::make_shared<Process>(Environment::findDependency("java"), std::vector<std::string>{ getJavaRamString(maxServerRamInGB), "-jar", JAVA_SERVER_FILE_EXTRACTED.string(), "nogui" }, m_directory);
            break;
        case Edition::Bedrock:
            m_proc = std::make_shared<Process>(BEDROCK_SERVER_FILE_EXTRACTED);
            break;
        case Edition::Forge:
            m_proc = std::make_shared<Process>(Environment::findDependency("java"), std::vector<std::string>{ getJavaRamString(maxServerRamInGB), "-jar", getForgeShimFile(m_directory).string(), "nogui" }, m_directory);
            break;
        }
        if(m_proc->start())
        {
            s_serverRunning = true;
            return true;
        }
        m_proc.reset();
        return false;
    }

    bool Server::stop()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_proc)
        {
            return true;
        }
        m_proc->sendCommand("stop");
        m_proc->waitForExit();
        if(m_broadcaster)
        {
            m_broadcaster->stop();
        }
        if(!m_proc->isRunning())
        {
            s_serverRunning = false;
            m_proc.reset();
            m_broadcaster.reset();
            return true;
        }
        return false;
    }

    bool Server::command(const std::string& cmd)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(!m_proc)
        {
            return false;
        }
        return m_proc->sendCommand(cmd);
    }

    const ServerAddress& Server::broadcast(const std::string& ngrokToken)
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        static ServerAddress empty;
        if(!m_proc || ngrokToken.empty())
        {
            return empty;
        }
        if(m_broadcaster)
        {
            return m_broadcaster->getAddress();
        }
        m_broadcaster = std::make_shared<Broadcaster>(m_properties.getServerPort(), ngrokToken);
        if(m_broadcaster->start().isEmpty())
        {
            m_broadcaster.reset();
            return empty;
        }
        return m_broadcaster->getAddress();
    }

    boost::json::object Server::toJson() const
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        boost::json::object json;
        json["Version"] = m_version.toJson();
        json["Properties"] = m_properties.toJson();
        json["Directory"] = m_directory.string();
        return json;
    }

    bool Server::wrtieFilesToDisk()
    {
        std::ofstream json{ MINIERA_FILE, std::ios::trunc };
        json << toJson() << std::endl;
        std::lock_guard<std::mutex> lock{ m_mutex };
        std::ofstream prop{ SERVER_PROPERTIES_FILE, std::ios::trunc };
        prop << m_properties.toString() << std::endl;
        if(m_version.getEdition() != Edition::Bedrock)
        {
            std::ofstream eula{ EULA_FILE, std::ios::trunc };
            eula << "eula=true" << std::endl;
            return json.is_open() && prop.is_open() && eula.is_open();
        }
        return json.is_open() && prop.is_open();
    }

    bool Server::initializeCheck()
    {
        std::lock_guard<std::mutex> lock{ m_mutex };
        if(m_initialized)
        {
            return m_initialized;
        }
        std::filesystem::create_directories(m_directory);
        if(m_version.getEdition() == Edition::Java)
        {
            m_initialized = std::filesystem::exists(JAVA_SERVER_FILE_EXTRACTED) && std::filesystem::exists(EULA_FILE) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        else if(m_version.getEdition() == Edition::Bedrock)
        {
            m_initialized = std::filesystem::exists(BEDROCK_SERVER_FILE_EXTRACTED) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        else if(m_version.getEdition() == Edition::Forge)
        {
            m_initialized = std::filesystem::exists(Environment::getOperatingSystem() == OperatingSystem::Windows ? "run.bat" : "run.sh") && std::filesystem::exists(EULA_FILE) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        return m_initialized;
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
        if(m_version.getEdition() == Edition::Java)
        {
            boost::json::value json = Web::fetchJson(m_version.getReleaseUrl());
            if(json.is_object())
            {
                downloadSuccessful = Web::downloadFile(json.as_object()["downloads"].as_object()["server"].as_object()["url"].as_string().c_str(), JAVA_SERVER_FILE_RAW, downloadProgress);
            }
        }
        else if(m_version.getEdition() == Edition::Bedrock)
        {
            downloadSuccessful = Web::downloadFile(m_version.getReleaseUrl(), BEDROCK_SERVER_FILE_RAW, downloadProgress);
        }
        else if(m_version.getEdition() == Edition::Forge)
        {
            downloadSuccessful = Web::downloadFile(m_version.getReleaseUrl(), FORGE_SERVER_FILE_RAW, downloadProgress);
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
        if(m_version.getEdition() == Edition::Java)
        {
            Process proc{ Environment::findDependency("java"), std::vector<std::string>{ "-jar", JAVA_SERVER_FILE_RAW.string(), "--initSettings" }, m_directory };
            proc.start();
            proc.waitForExit();
            extractionSuccessful = proc.getExitCode() == 0;
        }
        else if(m_version.getEdition() == Edition::Bedrock)
        {
            std::unique_ptr<ZipFile> zip{ std::make_unique<ZipFile>(BEDROCK_SERVER_FILE_RAW) };
            std::function<void(double)> extractionProgress{ [&](double progress)
            {
                m_initializationProgressChanged.invoke({ getName(), progress, log, false, false });
            }};
            extractionSuccessful = zip->extract(m_directory, extractionProgress);
            zip.reset();
            std::filesystem::remove(BEDROCK_SERVER_FILE_RAW);
            if(Environment::getOperatingSystem() == OperatingSystem::Windows)
            {
                //Prevents UWP lookback when connecting to server from client on same host
                Environment::exec("CheckNetIsolation.exe LoopbackExempt -a -p=S-1-15-2-1958404141-86561845-1752920682-3514627264-368642714-62675701-733520436");
            }
        }
        else if(m_version.getEdition() == Edition::Forge)
        {
            Process proc{ Environment::findDependency("java"), std::vector<std::string>{ "-jar", FORGE_SERVER_FILE_RAW.string(), "--installServer" }, m_directory };
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
        log += _("[Write] Starting server property files writing...\n");
        m_initializationProgressChanged.invoke({ getName(), 0.0, log, false, false });
        if(wrtieFilesToDisk())
        {
            log += _("[Write] Server propety files writing completed.\n");
            return true;
        }
        else
        {
            log += _("[Write] Error writing server property files.\n");
            m_initializationProgressChanged.invoke({ getName(), 1.0, log, true, true });
            return false;
        }
    }
}
