#include "views/mainwindow.h"
#include <QAction>
#include <QComboBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <libnick/helpers/codehelpers.h>
#include <libnick/localization/gettext.h>
#include <oclero/qlementine/widgets/ActionButton.hpp>
#include "controls/aboutdialog.h"
#include "controls/infobar.h"
#include "controls/serverinitializationdialog.h"
#include "controls/statuspage.h"
#include "helpers/qthelpers.h"
#include "views/newserverdialog.h"
#include "views/serverpage.h"
#include "views/settingsdialog.h"

using namespace Nickvision::App;
using namespace Nickvision::Miniera::Qt::Controls;
using namespace Nickvision::Miniera::Qt::Helpers;
using namespace Nickvision::Miniera::Shared::Controllers;
using namespace Nickvision::Miniera::Shared::Events;
using namespace Nickvision::Miniera::Shared::Models;
using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Notifications;
using namespace Nickvision::Update;
using namespace oclero::qlementine;

namespace Ui
{
    class MainWindow
    {
    public:
        void setupUi(Nickvision::Miniera::Qt::Views::MainWindow* parent) 
        {
            tabs = new QTabWidget(parent);
            tabs->setDocumentMode(true);
            //Actions
            actionNewServer = new QAction(parent);
            actionNewServer->setText(_("New Server"));
            actionNewServer->setIcon(QLEMENTINE_ICON(Action_AddFile));
            actionNewServer->setShortcut(Qt::CTRL | Qt::Key_N);
            actionLoadServer = new QAction(parent);
            actionLoadServer->setText(_("Load Server"));
            actionLoadServer->setIcon(QLEMENTINE_ICON(Document_Open));
            actionLoadServer->setShortcut(Qt::CTRL | Qt::Key_O);
            actionDeleteServer = new QAction(parent);
            actionDeleteServer->setText(_("Delete Server"));
            actionDeleteServer->setIcon(QLEMENTINE_ICON(Action_Trash));
            actionDeleteServer->setShortcut(Qt::CTRL | Qt::Key_Delete);
            actionDeleteServer->setEnabled(false);
            actionExit = new QAction(parent);
            actionExit->setText(_("Exit"));
            actionExit->setIcon(QLEMENTINE_ICON(Action_Close));
            actionExit->setShortcut(Qt::CTRL | Qt::Key_Q);
            actionStartStop = new QAction(parent);
            actionStartStop->setText(_("Start/Stop"));
            actionStartStop->setIcon(QLEMENTINE_ICON(Action_OnOff));
            actionStartStop->setShortcut(Qt::Key_F5);
            actionStartStop->setEnabled(false);
            actionBroadcast = new QAction(parent);
            actionBroadcast->setText(_("Broadcast"));
            actionBroadcast->setIcon(QLEMENTINE_ICON(Misc_Globe));
            actionBroadcast->setShortcut(Qt::CTRL | Qt::Key_B);
            actionBroadcast->setEnabled(false);
            actionSettings = new QAction(parent);
            actionSettings->setText(_("Settings"));
            actionSettings->setIcon(QLEMENTINE_ICON(Navigation_Settings));
            actionSettings->setShortcut(Qt::CTRL | Qt::Key_Comma);
            actionCheckForUpdates = new QAction(parent);
            actionCheckForUpdates->setText(_("Check for Updates"));
            actionCheckForUpdates->setIcon(QLEMENTINE_ICON(Action_Update));
            actionGitHubRepo = new QAction(parent);
            actionGitHubRepo->setText(_("GitHub Repo"));
            actionGitHubRepo->setIcon(QLEMENTINE_ICON(Software_VersionControl));
            actionReportABug = new QAction(parent);
            actionReportABug->setText(_("Report a Bug"));
            actionReportABug->setIcon(QLEMENTINE_ICON(Misc_Debug));
            actionDiscussions = new QAction(parent);
            actionDiscussions->setText(_("Discussions"));
            actionDiscussions->setIcon(QLEMENTINE_ICON(Misc_Users));
            actionAbout = new QAction(parent);
            actionAbout->setText(_("About Miniera"));
            actionAbout->setIcon(QLEMENTINE_ICON(Misc_Info));
            actionAbout->setShortcut(Qt::Key_F1);
            //InfoBar
            infoBar = new Nickvision::Miniera::Qt::Controls::InfoBar(parent);
            parent->addDockWidget(::Qt::BottomDockWidgetArea, infoBar);
            //InitDialog
            initDialog = new Nickvision::Miniera::Qt::Controls::ServerInitializationDialog(parent);
            //MenuBar
            QMenu* menuFile{ new QMenu(parent) };
            menuFile->setTitle(_("File"));
            menuFile->addAction(actionNewServer);
            menuFile->addAction(actionLoadServer);
            menuFile->addSeparator();
            menuFile->addAction(actionDeleteServer);
            menuFile->addSeparator();
            menuFile->addAction(actionExit);
            QMenu* menuEdit{ new QMenu(parent) };
            menuEdit->setTitle(_("Edit"));
            menuEdit->addAction(actionSettings);
            QMenu* menuServer{ new QMenu(parent) };
            menuServer->setTitle(_("Server"));
            menuServer->addAction(actionStartStop);
            menuServer->addAction(actionBroadcast);
            QMenu* menuHelp{ new QMenu(parent) };
            menuHelp->setTitle(_("Help"));
            menuHelp->addAction(actionCheckForUpdates);
            menuHelp->addSeparator();
            menuHelp->addAction(actionGitHubRepo);
            menuHelp->addAction(actionReportABug);
            menuHelp->addAction(actionDiscussions);
            menuHelp->addSeparator();
            menuHelp->addAction(actionAbout);
            parent->menuBar()->addMenu(menuFile);
            parent->menuBar()->addMenu(menuEdit);
            parent->menuBar()->addMenu(menuServer);
            parent->menuBar()->addMenu(menuHelp);
            //Home Page
            Nickvision::Miniera::Qt::Controls::StatusPage* pageHome{ new Nickvision::Miniera::Qt::Controls::StatusPage(parent) };
            pageHome->setIcon(QLEMENTINE_ICON(Hardware_Gamepad));
            pageHome->setTitle(_("Manage a Server"));
            pageHome->setDescription(_("Load or create a new server to get started"));
            ActionButton* btnNewServer = new ActionButton(parent);
            btnNewServer->setAutoDefault(false);
            btnNewServer->setDefault(false);
            btnNewServer->setMinimumWidth(300);
            btnNewServer->setText(_("New Server"));
            btnNewServer->setIcon(QLEMENTINE_ICON(Action_AddFile));
            btnNewServer->setAction(actionNewServer);
            pageHome->addWidget(btnNewServer);
            ActionButton* btnLoadServer = new ActionButton(parent);
            btnLoadServer->setMinimumWidth(300);
            btnLoadServer->setText(_("Load Server"));
            btnLoadServer->setIcon(QLEMENTINE_ICON(Document_Open));
            btnLoadServer->setAction(actionLoadServer);
            pageHome->addWidget(btnLoadServer);
            tabs->addTab(pageHome, QLEMENTINE_ICON(Navigation_Home), _("Home"));
            //Main Layout
            QObject::connect(tabs, &QTabWidget::currentChanged, [this](int current)
            {
                actionDeleteServer->setEnabled(current != 0);
                actionStartStop->setEnabled(current != 0);
                actionBroadcast->setEnabled(current != 0);
            });
            parent->setCentralWidget(tabs);
        }

        QAction* actionNewServer;
        QAction* actionLoadServer;
        QAction* actionDeleteServer;
        QAction* actionExit;
        QAction* actionSettings;
        QAction* actionStartStop;
        QAction* actionBroadcast;
        QAction* actionCheckForUpdates;
        QAction* actionGitHubRepo;
        QAction* actionReportABug;
        QAction* actionDiscussions;
        QAction* actionAbout;
        Nickvision::Miniera::Qt::Controls::InfoBar* infoBar;
        QTabWidget* tabs;
        Nickvision::Miniera::Qt::Controls::ServerInitializationDialog* initDialog;
    };
}

namespace Nickvision::Miniera::Qt::Views
{
    MainWindow::MainWindow(const std::shared_ptr<MainWindowController>& controller, oclero::qlementine::ThemeManager* themeManager, QWidget* parent) 
        : QMainWindow{ parent },
        m_ui{ new Ui::MainWindow() },
        m_controller{ controller },
        m_themeManager{ themeManager },
        m_confirmClose{ false }
    {
        //Window Settings
        bool stable{ m_controller->getAppInfo().getVersion().getVersionType() == VersionType::Stable };
        setWindowTitle(stable ? _("Miniera") : _("Miniera (Preview)"));
        setWindowIcon(QIcon(":/icon.ico"));
        setAcceptDrops(true);
        //Load Ui
        m_ui->setupUi(this);
        //Signals
        connect(m_ui->actionNewServer, &QAction::triggered, this, &MainWindow::newServer);
        connect(m_ui->actionLoadServer, &QAction::triggered, this, &MainWindow::loadServer);
        connect(m_ui->actionExit, &QAction::triggered, this, &MainWindow::close);
        connect(m_ui->actionSettings, &QAction::triggered, this, &MainWindow::settings);
        connect(m_ui->actionStartStop, &QAction::triggered, this, &MainWindow::startStop);
        connect(m_ui->actionBroadcast, &QAction::triggered, this, &MainWindow::broadcast);
        connect(m_ui->actionDeleteServer, &QAction::triggered, this, &MainWindow::deleteServer);
        connect(m_ui->actionCheckForUpdates, &QAction::triggered, this, &MainWindow::checkForUpdates);
        connect(m_ui->actionGitHubRepo, &QAction::triggered, this, &MainWindow::gitHubRepo);
        connect(m_ui->actionReportABug, &QAction::triggered, this, &MainWindow::reportABug);
        connect(m_ui->actionDiscussions, &QAction::triggered, this, &MainWindow::discussions);
        connect(m_ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
        m_controller->notificationSent() += [this](const NotificationSentEventArgs& args) { QtHelpers::dispatchToMainThread([this, args]() { onNotificationSent(args); }); };
        m_controller->serverLoaded() += [this](const ServerLoadedEventArgs& args) { QtHelpers::dispatchToMainThread([this, args]() { onServerLoaded(args); }); };
        m_controller->serverInitializationProgressChanged() += [this](const ServerInitializationProgressChangedEventArgs& args) { QtHelpers::dispatchToMainThread([this, args]() { onServerInitializationProgressChanged(args); }); };
    }

    MainWindow::~MainWindow()
    {
        delete m_ui;
    }

    void MainWindow::show()
    {
        QMainWindow::show();
#ifdef _WIN32
        const StartupInformation& info{ m_controller->startup(reinterpret_cast<HWND>(winId())) };
#elif defined(__linux__)
        const StartupInformation& info{ m_controller->startup(m_controller->getAppInfo().getId() + ".desktop") };
#else
        const StartupInformation& info{ m_controller->startup() };
#endif
        setGeometry(QWidget::geometry().x(), QWidget::geometry().y(), info.getWindowGeometry().getWidth(), info.getWindowGeometry().getHeight());
        if(info.getWindowGeometry().isMaximized())
        {
            showMaximized();
        }
    }

    void MainWindow::closeEvent(QCloseEvent* event)
    {
        if(!m_controller->canShutdown() && !m_confirmClose)
        {
            event->ignore();
            QMessageBox msgBox{ QMessageBox::Icon::Warning, QString::fromStdString(m_controller->getAppInfo().getShortName()), _("There is a server running. Closing the app will kill the server. Are you sure you want to exit?"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, this };
            if(msgBox.exec() == QMessageBox::StandardButton::Yes)
            {
                m_confirmClose = true;
                close();
            }
            return;
        }
        m_controller->shutdown({ geometry().width(), geometry().height(), isMaximized() });
        event->accept();
    }

    void MainWindow::newServer()
    {
        NewServerDialog dialog{ m_controller->createNewServerDialogController(), this };
        dialog.exec();
    }

    void MainWindow::loadServer()
    {
        std::vector<std::string> serverNames{ m_controller->getAvailableServerNames() };
        if(serverNames.empty())
        {
            return;
        }
        QStringList serverStrings;
        for(const std::string& server : serverNames)
        {
            serverStrings.push_back(QString::fromStdString(server));
        }
        bool ok;
        QString selected{ QInputDialog::getItem(this, _("Load Server"), _("Select a server:"), serverStrings, 0, false, &ok) };
        if(ok && !selected.isEmpty())
        {
            m_controller->loadServer(selected.toStdString());
        }
    }

    void MainWindow::settings()
    {
        SettingsDialog dialog{ m_controller->createPreferencesViewController(), m_themeManager, this };
        dialog.exec();
    }

    void MainWindow::startStop()
    {
        ServerPage* serverPage{ dynamic_cast<ServerPage*>(m_ui->tabs->currentWidget()) };
        if(serverPage)
        {
            serverPage->startStop();
        }
    }

    void MainWindow::broadcast()
    {
        ServerPage* serverPage{ dynamic_cast<ServerPage*>(m_ui->tabs->currentWidget()) };
        if(serverPage)
        {
            serverPage->broadcast();
        }
    }

    void MainWindow::deleteServer()
    {
        ServerPage* serverPage{ dynamic_cast<ServerPage*>(m_ui->tabs->currentWidget()) };
        if(serverPage)
        {
            QMessageBox msgBox{ QMessageBox::Icon::Warning, _("Delete Server?"), _("Are you sure you want to delete this server? This action is irreversible."), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, this };
            if(msgBox.exec() == QMessageBox::StandardButton::Yes)
            {
                m_controller->deleteServer(m_ui->tabs->tabText(m_ui->tabs->currentIndex()).toStdString());
                m_ui->tabs->removeTab(m_ui->tabs->currentIndex());
            }
        }
    }

    void MainWindow::checkForUpdates()
    {
        m_controller->checkForUpdates();
    }

#ifdef _WIN32
    void MainWindow::windowsUpdate()
    {
        m_controller->windowsUpdate();
    }
#endif

    void MainWindow::gitHubRepo()
    {
        QDesktopServices::openUrl(QString::fromStdString(m_controller->getAppInfo().getSourceRepo()));
    }

    void MainWindow::reportABug()
    {
        QDesktopServices::openUrl(QString::fromStdString(m_controller->getAppInfo().getIssueTracker()));
    }

    void MainWindow::discussions()
    {
        QDesktopServices::openUrl(QString::fromStdString(m_controller->getAppInfo().getSupportUrl()));
    }

    void MainWindow::about()
    {
        std::string extraDebug;
        extraDebug += "Qt " + std::string(qVersion()) + "\n";
        AboutDialog dialog{ m_controller->getAppInfo(), m_controller->getDebugInformation(extraDebug), this };
        dialog.exec();
    }

    void MainWindow::onNotificationSent(const NotificationSentEventArgs& args)
    {
        QString actionText;
        std::function<void()> actionCallback;
#ifdef _WIN32
        if(args.getAction() == "update")
        {
            actionText = _("Update");
            actionCallback = [this]() { windowsUpdate(); };
        }
#endif
        m_ui->infoBar->show(args, actionText, actionCallback);
    }

    void MainWindow::onServerInitializationProgressChanged(const ServerInitializationProgressChangedEventArgs& args)
    {
        m_ui->initDialog->updateProgress(args);
    }

    void MainWindow::onServerLoaded(const ServerLoadedEventArgs& args)
    {
        m_ui->tabs->addTab(new ServerPage(args.getServerViewController(), this), QLEMENTINE_ICON(Hardware_Server), QString::fromStdString(args.getName()));
        m_ui->tabs->setCurrentIndex(m_ui->tabs->count() - 1);
    }
}
