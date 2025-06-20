﻿#include "controllers/mainwindowcontroller.h"
#include <sstream>
#include <thread>
#include <libnick/filesystem/userdirectories.h>
#include <libnick/helpers/codehelpers.h>
#include <libnick/helpers/stringhelpers.h>
#include <libnick/localization/gettext.h>
#include <libnick/notifications/appnotification.h>
#include <libnick/system/environment.h>
#include "models/configuration.h"

using namespace Nickvision::App;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Miniera::Shared::Models;
using namespace Nickvision::Events;
using namespace Nickvision::Filesystem;
using namespace Nickvision::Helpers;
using namespace Nickvision::Notifications;
using namespace Nickvision::System;
using namespace Nickvision::Update;

namespace Nickvision::Miniera::Shared::Controllers
{
    MainWindowController::MainWindowController(const std::vector<std::string>& args)
        : m_started{ false },
        m_args{ args },
        m_appInfo{ "org.nickvision.miniera", "Nickvision Miniera", "Miniera" },
        m_dataFileManager{ m_appInfo.getName() },
        m_serverManager{ m_appInfo.getName(), m_dataFileManager.get<Configuration>("config") }
    {
        m_appInfo.setVersion({ "2025.3.0-alpha2" });
        m_appInfo.setShortName(_("Miniera"));
        m_appInfo.setDescription(_("Run Minecraft servers"));
        m_appInfo.setChangelog("- Initial Release");
        m_appInfo.setSourceRepo("https://github.com/NickvisionApps/Miniera");
        m_appInfo.setIssueTracker("https://github.com/NickvisionApps/Miniera/issues/new");
        m_appInfo.setSupportUrl("https://github.com/NickvisionApps/Miniera/discussions");
        m_appInfo.getExtraLinks()[_("Matrix Chat")] = "https://matrix.to/#/#nickvision:matrix.org";
        m_appInfo.getDevelopers()["Nicholas Logozzo"] = "https://github.com/nlogozzo";
        m_appInfo.getDevelopers()[_("Contributors on GitHub")] = "https://github.com/NickvisionApps/Miniera/graphs/contributors";
        m_appInfo.getDesigners()["Nicholas Logozzo"] = "https://github.com/nlogozzo";
        m_appInfo.getDesigners()[_("Fyodor Sobolev")] = "https://github.com/fsobolev";
        m_appInfo.getDesigners()["DaPigGuy"] = "https://github.com/DaPigGuy";
        m_appInfo.getArtists()[_("David Lapshin")] = "https://github.com/daudix";
        m_appInfo.setTranslatorCredits(_("translator-credits"));
        Localization::Gettext::init(m_appInfo.getEnglishShortName());
#ifdef _WIN32
        m_updater = std::make_shared<Updater>(m_appInfo.getSourceRepo());
#endif
    }

    Event<EventArgs>& MainWindowController::configurationSaved()
    {
        return m_dataFileManager.get<Configuration>("config").saved();
    }

    Event<NotificationSentEventArgs>& MainWindowController::notificationSent()
    {
        return AppNotification::sent();
    }

    Event<ServerLoadedEventArgs>& MainWindowController::serverLoaded()
    {
        return m_serverManager.serverLoaded();
    }

    Event<ParamEventArgs<std::string>>& MainWindowController::serverDeleted()
    {
        return m_serverManager.serverDeleted();
    }

    Event<ServerInitializationProgressChangedEventArgs>& MainWindowController::serverInitializationProgressChanged()
    {
        return m_serverManager.serverInitializationProgressChanged();
    }

    const AppInfo& MainWindowController::getAppInfo() const
    {
        return m_appInfo;
    }

    Theme MainWindowController::getTheme()
    {
        return m_dataFileManager.get<Configuration>("config").getTheme();
    }

    std::string MainWindowController::getDebugInformation(const std::string& extraInformation) const
    {
        std::stringstream builder;
        //ngrok
        if(Environment::findDependency("ngrok").empty())
        {
            builder << "ngrok not found" << std::endl;
        }
        else
        {
            builder << Environment::exec("\"" + Environment::findDependency("ngrok").string() + "\"" + " --version");
        }
        //Java
        if(Environment::findDependency("java").empty())
        {
            builder << "Java not found" << std::endl << std::endl;
        }
        else
        {
            builder << Environment::exec("\"" + Environment::findDependency("java").string() + "\"" + " -version") << std::endl;
        }
        //Extra
        if(!extraInformation.empty())
        {
            builder << extraInformation << std::endl;
#ifdef __linux__
            builder << Environment::exec("locale");
#endif
        }
        return Environment::getDebugInformation(m_appInfo, builder.str());
    }

    bool MainWindowController::canShutdown() const
    {
        return !Server::isServerRunning();
    }

    std::shared_ptr<NewServerDialogController> MainWindowController::createNewServerDialogController()
    {
        return std::make_shared<NewServerDialogController>(m_serverManager);
    }

    std::shared_ptr<PreferencesViewController> MainWindowController::createPreferencesViewController()
    {
        return std::make_shared<PreferencesViewController>(m_dataFileManager.get<Configuration>("config"));
    }

#ifdef _WIN32
    const StartupInformation& MainWindowController::startup(HWND hwnd)
#elif defined(__linux__)
    const StartupInformation& MainWindowController::startup(const std::string& desktopFile)
#else
    const StartupInformation& MainWindowController::startup()
#endif
    {
        static StartupInformation info;
        if (m_started)
        {
            return info;
        }
        //Load configuration
        info.setWindowGeometry(m_dataFileManager.get<Configuration>("config").getWindowGeometry());
        //Load taskbar item
#ifdef _WIN32
        m_taskbar.connect(hwnd);
        if (m_dataFileManager.get<Configuration>("config").getAutomaticallyCheckForUpdates())
        {
            checkForUpdates();
        }
#elif defined(__linux__)
        m_taskbar.connect(desktopFile);
#endif
        m_started = true;
        return info;
    }

    void MainWindowController::shutdown(const WindowGeometry& geometry)
    {
        Configuration& config{ m_dataFileManager.get<Configuration>("config") };
        config.setWindowGeometry(geometry);
        config.save();
    }

    void MainWindowController::checkForUpdates()
    {
        if(!m_updater)
        {
            return;
        }
        std::thread worker{ [this]()
        {
            Version latest{ m_updater->fetchCurrentVersion(VersionType::Stable) };
            if(!latest.empty())
            {
                if(latest > m_appInfo.getVersion())
                {
                    AppNotification::send({ _("New update available"), NotificationSeverity::Success, "update" });
                }
            }
        } };
        worker.detach();
    }

#ifdef _WIN32
    void MainWindowController::windowsUpdate()
    {
        if(!m_updater)
        {
            return;
        }
        AppNotification::send({ _("The update is downloading in the background and will start once it finishes"), NotificationSeverity::Informational });
        std::thread worker{ [this]()
        {
            if(!m_updater->windowsUpdate(VersionType::Stable))
            {
                AppNotification::send({ _("Unable to download and install update"), NotificationSeverity::Error });
            }
        } };
        worker.detach();
    }
#endif

    std::vector<std::string> MainWindowController::getAvailableServerNames()
    {
        return m_serverManager.getAvailableServersNames();
    }

    void MainWindowController::loadServer(const std::string& serverName)
    {
        if(!m_serverManager.loadServer(serverName))
        {
            AppNotification::send({ _("Server already loaded"), NotificationSeverity::Warning });
        }
    }

    void MainWindowController::deleteServer(const std::string& serverName)
    {
        if(!m_serverManager.deleteServer(serverName))
        {
            AppNotification::send({ _("Unable to delete the server as it is running"), NotificationSeverity::Error });
        }
    }
}
