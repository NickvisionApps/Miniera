#include "helpers/serverinitializationhelpers.h"
#include <format>
#include <fstream>
#include <functional>
#include <libnick/helpers/codehelpers.h>
#include <libnick/localization/gettext.h>
#include <libnick/network/web.h>
#include <libnick/system/environment.h>
#include <libnick/system/process.h>
#include "models/zipfile.h"

#define CONTINUE_DOWNLOAD 0
#define SERVER_NAME dir.stem().string()
#define SERVER_PROPERTIES_FILE (dir / "server.properties")
#define EULA_FILE (dir / "eula.txt")
#define JAVA_SERVER_FILE_RAW (dir / "server.jar")
#define JAVA_SERVER_FILE_EXTRACTED (dir / "server.jar")
#define BEDROCK_SERVER_FILE_RAW (dir / "server.zip")
#ifdef _WIN32
#define BEDROCK_SERVER_FILE_EXTRACTED (dir / "bedrock_server.exe")
#else
#define BEDROCK_SERVER_FILE_EXTRACTED (dir / "bedrock_server")
#endif
#define FORGE_SERVER_FILE_RAW (dir / "installer.jar")
#ifdef _WIN32
#define FORGE_SERVER_FILE_EXTRACTED (dir / "run.bat")
#else
#define FORGE_SERVER_FILE_EXTRACTED (dir / "run.sh")
#endif

using namespace Nickvision::Helpers;
using namespace Nickvision::Miniera::Shared::Models;
using namespace Nickvision::Network;
using namespace Nickvision::System;

namespace Nickvision::Miniera::Shared::Helpers
{
    bool ServerInitializationHelpers::check(const std::filesystem::path& dir, const ServerVersion& version)
    {
        bool initalized{ false };
        std::filesystem::create_directories(dir);
        if(version.getEdition() == Edition::Java)
        {
            initalized = std::filesystem::exists(JAVA_SERVER_FILE_EXTRACTED) && std::filesystem::exists(EULA_FILE) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        else if(version.getEdition() == Edition::Bedrock)
        {
            initalized = std::filesystem::exists(BEDROCK_SERVER_FILE_EXTRACTED) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        else if(version.getEdition() == Edition::Forge)
        {
            initalized = std::filesystem::exists(FORGE_SERVER_FILE_EXTRACTED) && std::filesystem::exists(EULA_FILE) && std::filesystem::exists(SERVER_PROPERTIES_FILE);
        }
        return initalized;
    }

    bool ServerInitializationHelpers::download(SERVER_INITIALIZATION_HELPER_ARGS)
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
                progressChanged.invoke({ SERVER_NAME, static_cast<double>(oldNow) / static_cast<double>(oldTotal), log, false, false });
            }
            return CONTINUE_DOWNLOAD;
        } };
        log += _("[Download] Starting server files download...\n");
        progressChanged.invoke({ SERVER_NAME, 0.0, log, false, false });
        if(version.getEdition() == Edition::Java)
        {
            boost::json::value json = Web::fetchJson(version.getReleaseUrl());
            if(json.is_object())
            {
                downloadSuccessful = Web::downloadFile(json.as_object()["downloads"].as_object()["server"].as_object()["url"].as_string().c_str(), JAVA_SERVER_FILE_RAW, downloadProgress);
            }
        }
        else if(version.getEdition() == Edition::Bedrock)
        {
            downloadSuccessful = Web::downloadFile(version.getReleaseUrl(), BEDROCK_SERVER_FILE_RAW, downloadProgress);
        }
        else if(version.getEdition() == Edition::Forge)
        {
            downloadSuccessful = Web::downloadFile(version.getReleaseUrl(), FORGE_SERVER_FILE_RAW, downloadProgress);
        }
        if(downloadSuccessful)
        {
            log += _("[Download] Server files download completed.\n");
        }
        else
        {
            log += _("[Download] Error downloading server files.\n");
            progressChanged.invoke({ SERVER_NAME, 1.0, log, true, true });
        }
        return downloadSuccessful;
    }

    bool ServerInitializationHelpers::extract(SERVER_INITIALIZATION_HELPER_ARGS)
    {
        bool extractionSuccessful{ false };
        log += _("[Extract] Starting server files extraction...\n");
        progressChanged.invoke({ SERVER_NAME, 0.0, log, false, false });
        if(version.getEdition() == Edition::Java)
        {
            std::vector<std::string> args;
            args.push_back("-jar");
            args.push_back(JAVA_SERVER_FILE_RAW.string());
            args.push_back("--initSettings");
            Process proc{ Environment::findDependency("java"), args, dir };
            proc.start();
            proc.waitForExit();
            extractionSuccessful = proc.getExitCode() == 0;
        }
        else if(version.getEdition() == Edition::Bedrock)
        {
            std::unique_ptr<ZipFile> zip{ std::make_unique<ZipFile>(BEDROCK_SERVER_FILE_RAW) };
            extractionSuccessful = zip->extract(dir);
            zip.reset();
            std::filesystem::remove(BEDROCK_SERVER_FILE_RAW);
            if(Environment::getOperatingSystem() == OperatingSystem::Windows)
            {
                //Prevents UWP lookback when connecting to server from client on same host
                Environment::exec("CheckNetIsolation.exe LoopbackExempt -a -p=S-1-15-2-1958404141-86561845-1752920682-3514627264-368642714-62675701-733520436");
            }
        }
        else if(version.getEdition() == Edition::Forge)
        {
            std::vector<std::string> args;
            args.push_back("-jar");
            args.push_back(FORGE_SERVER_FILE_RAW.string());
            args.push_back("--installServer");
            Process proc{ Environment::findDependency("java"), args, dir };
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
            progressChanged.invoke({ SERVER_NAME, 1.0, log, true, true });
        }
        return extractionSuccessful;
    }

    bool ServerInitializationHelpers::write(SERVER_INITIALIZATION_HELPER_ARGS)
    {
        bool writingSuccessful{ false };
        log += _("[Write] Starting server property files writing...\n");
        progressChanged.invoke({ SERVER_NAME, 0.0, log, false, false });
        if(version.getEdition() == Edition::Java || version.getEdition() == Edition::Forge)
        {
            std::ofstream eula{ EULA_FILE, std::ios::trunc };
            std::ofstream prop{ SERVER_PROPERTIES_FILE, std::ios::trunc };
            if(eula.is_open())
            {
                eula << "eula=true" << std::endl;
                log += _("[Write] Wrote EULA file.\n");
            }
            if(prop.is_open())
            {
                prop << properties.toString() << std::endl;
                log += _("[Write] Wrote server properties file.\n");
            }
            writingSuccessful = eula.is_open() && prop.is_open();
            eula.close();
            prop.close();
        }
        else if(version.getEdition() == Edition::Bedrock)
        {
            std::ofstream prop{ SERVER_PROPERTIES_FILE, std::ios::trunc };
            if(prop.is_open())
            {
                prop << properties.toString() << std::endl;
                log += _("[Write] Wrote server properties file.\n");
            }
            writingSuccessful = prop.is_open();
            prop.close();
        }
        if(writingSuccessful)
        {
            log += _("[Write] Server propety files writing completed.\n");
        }
        else
        {
            log += _("[Write] Error writing server property files.\n");
            progressChanged.invoke({ SERVER_NAME, 1.0, log, true, true });
        }
        return writingSuccessful;
    }
}
