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
#define JAVA_SERVER_FILE serverDir / "server.jar"
#define BEDROCK_SERVER_FILE serverDir / "server.zip"
#define FORGE_SERVER_FILE serverDir / "installer.jar"

using namespace Nickvision::Helpers;
using namespace Nickvision::Miniera::Shared::Models;
using namespace Nickvision::Network;
using namespace Nickvision::System;

namespace Nickvision::Miniera::Shared::Helpers
{
    bool ServerInitializationHelpers::download(SERVER_INITIALIZATION_HELPER_ARGS)
    {
        bool downloadSuccessful{ false };
        std::function<int(curl_off_t, curl_off_t, curl_off_t, curl_off_t)> downloadProgress{ [&log, &name, &progressChanged](curl_off_t dltotal, curl_off_t dlnow, curl_off_t, curl_off_t)
        {
            log += std::vformat(_("[Download] {}% completed"), std::make_format_args(CodeHelpers::unmove((dlnow / dltotal) * 100)));
            progressChanged.invoke({ name, static_cast<double>(dlnow) / static_cast<double>(dltotal), log, false, false });
            return CONTINUE_DOWNLOAD;
        } };
        log += _("[Download] Starting server files download...");
        progressChanged.invoke({ name, 0.0, log, false, false });
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
            progressChanged.invoke({ name, 1.0, log, true, true });
        }
        return downloadSuccessful;
    }

    bool ServerInitializationHelpers::extract(SERVER_INITIALIZATION_HELPER_ARGS)
    {
        bool extractionSuccessful{ false };
        log += _("[Extract] Starting server files extraction...");
        if(version.getEdition() == Edition::Java)
        {
            extractionSuccessful = true;
        }
        else if(version.getEdition() == Edition::Bedrock)
        {
            ZipFile zip{ BEDROCK_SERVER_FILE };
            extractionSuccessful = zip.extract(serverDir);
        }
        else if(version.getEdition() == Edition::Forge)
        {
            //TODO: Run installer jar to extract server
        }
        if(extractionSuccessful)
        {
            log += _("[Extract] Server files extraction completed.");
        }
        else
        {
            log += _("[Extract] Error extracting server files.");
            progressChanged.invoke({ name, 1.0, log, true, true });
        }
        return extractionSuccessful;
    }

    bool ServerInitializationHelpers::write(SERVER_INITIALIZATION_HELPER_ARGS)
    {
        bool writingSuccessful{ false };
        log += _("[Write] Starting server property files writing...");
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
            progressChanged.invoke({ name, 1.0, log, true, true });
        }
        return writingSuccessful;
    }
}
