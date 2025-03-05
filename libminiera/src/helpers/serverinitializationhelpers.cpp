#include "helpers/serverinitializationhelpers.h"
#include <format>
#include <functional>
#include <libnick/helpers/codehelpers.h>
#include <libnick/localization/gettext.h>
#include <libnick/network/web.h>
#include <libnick/system/environment.h>
#include <libnick/system/process.h>
#include "models/zipfile.h"

#define CONTINUE_DOWNLOAD 0
#define SERVER_NAME dir.stem().string()
#define JAVA_SERVER_FILE (dir / "server.jar")
#define BEDROCK_SERVER_FILE (dir / "server.zip")
#define FORGE_SERVER_FILE (dir / "installer.jar")

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
        //TODO: Implement
        return initalized;
    }

    bool ServerInitializationHelpers::download(SERVER_INITIALIZATION_HELPER_ARGS)
    {
        bool downloadSuccessful{ false };
        std::function<int(curl_off_t, curl_off_t, curl_off_t, curl_off_t)> downloadProgress{ [&log, &dir, &progressChanged](curl_off_t dltotal, curl_off_t dlnow, curl_off_t, curl_off_t)
        {
            if(dltotal == 0)
            {
                dltotal = 1;
            }
            log += std::vformat(_("[Download] {}% completed"), std::make_format_args(CodeHelpers::unmove((dlnow / dltotal) * 100)));
            progressChanged.invoke({ SERVER_NAME, static_cast<double>(dlnow) / static_cast<double>(dltotal), log, false, false });
            return CONTINUE_DOWNLOAD;
        } };
        log += _("[Download] Starting server files download...");
        progressChanged.invoke({ SERVER_NAME, 0.0, log, false, false });
        if(version.getEdition() == Edition::Java)
        {
            boost::json::value json = Web::fetchJson(version.getReleaseUrl());
            if(json.is_object())
            {
                downloadSuccessful = Web::downloadFile(json.as_object()["downloads"].as_object()["server"].as_object()["url"].as_string().c_str(), JAVA_SERVER_FILE, downloadProgress);
            }
        }
        else if(version.getEdition() == Edition::Bedrock)
        {
            downloadSuccessful = Web::downloadFile(version.getReleaseUrl(), BEDROCK_SERVER_FILE, downloadProgress);
        }
        else if(version.getEdition() == Edition::Forge)
        {
            downloadSuccessful = Web::downloadFile(version.getReleaseUrl(), FORGE_SERVER_FILE, downloadProgress);
        }
        if(downloadSuccessful)
        {
            log += _("[Download] Server files download completed.");
        }
        else
        {
            log += _("[Download] Error downloading server files.");
            progressChanged.invoke({ SERVER_NAME, 1.0, log, true, true });
        }
        return downloadSuccessful;
    }

    bool ServerInitializationHelpers::extract(SERVER_INITIALIZATION_HELPER_ARGS)
    {
        bool extractionSuccessful{ false };
        log += _("[Extract] Starting server files extraction...");
        progressChanged.invoke({ SERVER_NAME, 0.0, log, false, false });
        if(version.getEdition() == Edition::Java)
        {
            //The Java edition downloads the ready-to-go server.jar file, no extraction needed.
            extractionSuccessful = true;
        }
        else if(version.getEdition() == Edition::Bedrock)
        {
            std::unique_ptr<ZipFile> zip{ std::make_unique<ZipFile>(BEDROCK_SERVER_FILE) };
            extractionSuccessful = zip->extract(dir);
            zip.reset();
            std::filesystem::remove(BEDROCK_SERVER_FILE);
        }
        else if(version.getEdition() == Edition::Forge)
        {
            std::vector<std::string> args;
            args.push_back("-jar");
            args.push_back(FORGE_SERVER_FILE.string());
            args.push_back("--installServer");
            args.push_back("\"" + dir.string() + "\"");
            Process proc{ Environment::findDependency("java"), args };
            proc.start();
            proc.waitForExit();
            extractionSuccessful = proc.getExitCode() == 0;
            std::filesystem::remove(FORGE_SERVER_FILE);
        }
        if(extractionSuccessful)
        {
            log += _("[Extract] Server files extraction completed.");
        }
        else
        {
            log += _("[Extract] Error extracting server files.");
            progressChanged.invoke({ SERVER_NAME, 1.0, log, true, true });
        }
        return extractionSuccessful;
    }

    bool ServerInitializationHelpers::write(SERVER_INITIALIZATION_HELPER_ARGS)
    {
        bool writingSuccessful{ false };
        log += _("[Write] Starting server property files writing...");
        progressChanged.invoke({ SERVER_NAME, 0.0, log, false, false });
        if(version.getEdition() == Edition::Java)
        {
            //TODO: Write eula file
            //TODO: Write properties file
        }
        else if(version.getEdition() == Edition::Bedrock)
        {
            //TODO: Write properties file
        }
        else if(version.getEdition() == Edition::Forge)
        {
            //TODO: Write eula file
            //TODO: Write properties file
        }
        if(writingSuccessful)
        {
            log += _("[Write] Server propety files writing completed.");
        }
        else
        {
            log += _("[Write] Error writing server property files.");
            progressChanged.invoke({ SERVER_NAME, 1.0, log, true, true });
        }
        return writingSuccessful;
    }
}
