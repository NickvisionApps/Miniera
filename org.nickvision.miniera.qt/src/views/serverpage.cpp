#include "views/serverpage.h"
#include <format>
#include <QFileDialog>
#include <QFont>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <libnick/helpers/sizehelpers.h>
#include <libnick/localization/gettext.h>
#include <oclero/qlementine/widgets/ActionButton.hpp>
#include <oclero/qlementine/widgets/LineEdit.hpp>
#include <oclero/qlementine/widgets/SegmentedControl.hpp>
#include "controls/statuspage.h"
#include "helpers/qthelpers.h"

using namespace Nickvision::Events;
using namespace Nickvision::Helpers;
using namespace Nickvision::Miniera::Qt::Controls;
using namespace Nickvision::Miniera::Qt::Helpers;
using namespace Nickvision::Miniera::Shared::Controllers;
using namespace Nickvision::Miniera::Shared::Models;
using namespace oclero::qlementine;

namespace Ui
{
    class ServerPage
    {
    public:
        void setupUi(Nickvision::Miniera::Qt::Views::ServerPage* parent, bool supportsMods, double maxRamInMB)
        {
            QFont boldFont;
            boldFont.setBold(true);
            QFont boldFontBig;
            boldFontBig.setBold(true);
            boldFontBig.setPointSize(12);
            //Header
            lblName = new QLabel(parent);
            lblName->setFont(boldFontBig);
            lblVersion = new QLabel(parent);
            btnStartStop = new ActionButton(parent);
            btnStartStop->setAutoDefault(true);
            btnStartStop->setDefault(true);
            btnStartStop->setMinimumWidth(200);
            btnStartStop->setText(_("Start"));
            btnStartStop->setIcon(QLEMENTINE_ICON(Action_OnOff));
            btnBroadcast = new ActionButton(parent);
            btnBroadcast->setAutoDefault(false);
            btnBroadcast->setDefault(false);
            btnBroadcast->setEnabled(false);
            btnBroadcast->setMinimumWidth(200);
            btnBroadcast->setText(_("Broadcast"));
            btnBroadcast->setIcon(QLEMENTINE_ICON(Misc_Globe));
            QVBoxLayout* layoutInfo{ new QVBoxLayout() };
            layoutInfo->addWidget(lblName);
            layoutInfo->addWidget(lblVersion);
            QVBoxLayout* layoutActions{ new QVBoxLayout() };
            layoutActions->addWidget(btnStartStop);
            layoutActions->addWidget(btnBroadcast);
            QHBoxLayout* layoutHeader{ new QHBoxLayout() };
            layoutHeader->addLayout(layoutInfo);
            layoutHeader->addStretch();
            layoutHeader->addLayout(layoutActions);
            //Navigation
            navBar = new SegmentedControl(parent);
            navBar->addItem(_("Dashboard"), QLEMENTINE_ICON(Navigation_UiPanelTop));
            if(supportsMods)
            {
                navBar->addItem(_("Mods"), QLEMENTINE_ICON(Misc_Blocks));
            }
            navBar->addItem(_("Settings"), QLEMENTINE_ICON(Navigation_Settings));
            QHBoxLayout* layoutNavigation{ new QHBoxLayout() };
            layoutNavigation->addStretch();
            layoutNavigation->addWidget(navBar);
            layoutNavigation->addStretch();
            //Dashboard Page
            QLabel* lblUrlTitle{ new QLabel(_("URL:"), parent) };
            lblUrlTitle->setFont(boldFont);
            lblUrl = new QLabel(parent);
            QLabel* lblPortTitle{ new QLabel(_("Port:"), parent) };
            lblPortTitle->setFont(boldFont);
            lblPort = new QLabel(parent);
            QFormLayout* layoutAddress{ new QFormLayout() };
            layoutAddress->addRow(lblUrlTitle, lblUrl);
            layoutAddress->addRow(lblPortTitle, lblPort);
            QGroupBox* groupAddress{ new QGroupBox(_("Address"), parent) };
            groupAddress->setLayout(layoutAddress);
            QLabel* lblCPUTitle{ new QLabel(_("CPU:"), parent) };
            lblCPUTitle->setFont(boldFont);
            lblCPU = new QLabel(parent);
            QLabel* lblRAMTitle{ new QLabel(_("RAM:"), parent) };
            lblRAMTitle->setFont(boldFont);
            lblRAM = new QLabel(parent);
            QFormLayout* layoutResources{ new QFormLayout() };
            layoutResources->addRow(lblCPUTitle, lblCPU);
            layoutResources->addRow(lblRAMTitle, lblRAM);
            QGroupBox* groupResources{ new QGroupBox(_("Resources"), parent) };
            groupResources->setLayout(layoutResources);
            QHBoxLayout* layoutGroups{ new QHBoxLayout() };
            layoutGroups->addWidget(groupAddress);
            layoutGroups->addWidget(groupResources);
            lblOutput = new QLabel(parent);
            lblOutput->setAlignment(::Qt::AlignTop);
            lblOutput->setWordWrap(true);
            lblOutput->setTextInteractionFlags(::Qt::TextSelectableByMouse);
            lblOutput->setText(_("No console output"));
            txtCommand = new LineEdit(parent);
            txtCommand->setPlaceholderText(_("Enter command here"));
            QScrollArea* scrollConsole = new QScrollArea(parent);
            scrollConsole->setVerticalScrollBarPolicy(::Qt::ScrollBarAsNeeded);
            scrollConsole->setHorizontalScrollBarPolicy(::Qt::ScrollBarAlwaysOff);
            scrollConsole->setFrameShape(QFrame::Shape::NoFrame);
            scrollConsole->setWidgetResizable(true);
            scrollConsole->setWidget(lblOutput);
            QObject::connect(scrollConsole->verticalScrollBar(), &QScrollBar::rangeChanged, [this, scrollConsole](int, int max)
            {
                scrollConsole->verticalScrollBar()->setValue(max);
            });
            QVBoxLayout* layoutConsole{ new QVBoxLayout() };
            layoutConsole->addWidget(scrollConsole);
            layoutConsole->addWidget(txtCommand);
            QGroupBox* groupConsole{ new QGroupBox(_("Console"), parent) };
            groupConsole->setLayout(layoutConsole);
            QVBoxLayout* layoutDashboard{ new QVBoxLayout() };
            layoutDashboard->setContentsMargins(0, 0, 0, 0);
            layoutDashboard->addLayout(layoutGroups);
            layoutDashboard->addWidget(groupConsole);
            QWidget* pageDashboard = new QWidget(parent);
            pageDashboard->setLayout(layoutDashboard);
            //Mods Page
            btnUploadMod = new ActionButton(parent);
            btnUploadMod->setAutoDefault(false);
            btnUploadMod->setDefault(false);
            btnUploadMod->setMinimumWidth(140);
            btnUploadMod->setText(_("Upload"));
            btnUploadMod->setIcon(QLEMENTINE_ICON(Misc_CloudUp));
            btnRemoveMod = new ActionButton(parent);
            btnRemoveMod->setAutoDefault(false);
            btnRemoveMod->setDefault(false);
            btnRemoveMod->setMinimumWidth(140);
            btnRemoveMod->setText(_("Remove"));
            btnRemoveMod->setIcon(QLEMENTINE_ICON(Action_Trash));
            listMods = new QListWidget(parent);
            listMods->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
            listMods->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
            listMods->setDropIndicatorShown(false);
            QLabel* lblNewMods{ new QLabel(parent) };
            lblNewMods->setText(_("A restart is required for mods added after the server has been started."));
            QHBoxLayout* layoutModActions{ new QHBoxLayout() };
            layoutModActions->addWidget(btnUploadMod);
            layoutModActions->addWidget(btnRemoveMod);
            layoutModActions->addWidget(lblNewMods);
            layoutModActions->addStretch();
            QVBoxLayout* layoutMods{ new QVBoxLayout() };
            layoutMods->setContentsMargins(0, 0, 0, 0);
            layoutMods->addLayout(layoutModActions);
            layoutMods->addWidget(listMods);
            QWidget* pageMods = new QWidget(parent);
            pageMods->setLayout(layoutMods);
            //Settings Page
            Nickvision::Miniera::Qt::Controls::StatusPage* pageSettings{ new Nickvision::Miniera::Qt::Controls::StatusPage(parent) };
            pageSettings->setIcon(QLEMENTINE_ICON(Action_Build));
            pageSettings->setTitle(_("Coming Soon!"));
            pageSettings->setDescription("We are working hard to bring this feature to you soon");
            //View Stack
            viewStack = new QStackedWidget(parent);
            viewStack->addWidget(pageDashboard);
            viewStack->addWidget(pageMods);
            viewStack->addWidget(pageSettings);
            QObject::connect(navBar, &SegmentedControl::currentIndexChanged, [this]()
            {
                viewStack->setCurrentIndex(navBar->currentIndex());
            });
            //Main Layout
            QFrame* line{ new QFrame(parent) };
            line->setFrameShape(QFrame::Shape::HLine);
            line->setFrameShadow(QFrame::Shadow::Sunken);
            QVBoxLayout* layoutMain{ new QVBoxLayout(parent) };
            layoutMain->addLayout(layoutHeader);
            layoutMain->addLayout(layoutNavigation);
            layoutMain->addWidget(line);
            layoutMain->addWidget(viewStack);
            parent->setLayout(layoutMain);
        }

        QLabel* lblName;
        QLabel* lblVersion;
        ActionButton* btnStartStop;
        ActionButton* btnBroadcast;
        SegmentedControl* navBar;
        QStackedWidget* viewStack;
        QLabel* lblUrl;
        QLabel* lblPort;
        QLabel* lblCPU;
        QLabel* lblRAM;
        QLabel* lblOutput;
        LineEdit* txtCommand;
        ActionButton* btnUploadMod;
        ActionButton* btnRemoveMod;
        QListWidget* listMods;
    };
}

namespace Nickvision::Miniera::Qt::Views
{
    ServerPage::ServerPage(const std::shared_ptr<ServerViewController>& controller, QWidget* parent)
        : QWidget{ parent },
        m_ui{ new Ui::ServerPage() },
        m_controller{ controller }
    {
        //Load Ui
        ServerAddress address{ m_controller->getAddress() };
        m_ui->setupUi(this, m_controller->supportsMods(), static_cast<double>(m_controller->getMaxServerRamInMB()));
        m_ui->lblName->setText(QString::fromStdString(m_controller->getName()));
        m_ui->lblVersion->setText(QString::fromStdString(m_controller->getVersion()));
        m_ui->lblUrl->setText(QString::fromStdString(address.getUrl()));
        m_ui->lblPort->setText(QString::number(address.getPort()));
        m_ui->lblCPU->setText("0%");
        m_ui->lblRAM->setText(QString::fromStdString(m_controller->getRAMString(0L)));
        reloadMods();
        //Signals
        m_controller->powerChanged() += [this](const ParamEventArgs<PowerStatus>& args) { QtHelpers::dispatchToMainThread([this, args]() { onPowerChanged(args); }); };
        m_controller->addressChanged() += [this](const ParamEventArgs<ServerAddress>& args) { QtHelpers::dispatchToMainThread([this, args]() { onAddressChanged(args); }); };
        m_controller->consoleOutputChanged() += [this](const ParamEventArgs<std::string>& args) { QtHelpers::dispatchToMainThread([this, args]() { onConsoleOutputChanged(args); }); };
        m_controller->resourceUsageChanged() += [this](const ParamEventArgs<std::pair<double, unsigned long long>>& args) { QtHelpers::dispatchToMainThread([this, args]() { onResourceUsageChanged(args); }); };
        connect(m_ui->btnStartStop, &QPushButton::clicked, this, &ServerPage::startStop);
        connect(m_ui->btnBroadcast, &QPushButton::clicked, this, &ServerPage::broadcast);
        connect(m_ui->txtCommand, &QLineEdit::returnPressed, this, &ServerPage::sendCommand);
        connect(m_ui->btnUploadMod, &QPushButton::clicked, this, &ServerPage::uploadMod);
        connect(m_ui->btnRemoveMod, &QPushButton::clicked, this, &ServerPage::removeMod);
    }

    ServerPage::~ServerPage()
    {
        delete m_ui;
    }

    void ServerPage::startStop()
    {
        m_ui->btnStartStop->setText(_("Loading..."));
        m_ui->btnStartStop->setEnabled(false);
        m_controller->startStop();
    }

    void ServerPage::broadcast()
    {
        m_ui->btnBroadcast->setText(_("Loading..."));
        m_ui->btnBroadcast->setEnabled(false);
        m_controller->broadcast();
    }

    void ServerPage::sendCommand()
    {
        m_controller->sendCommand(m_ui->txtCommand->text().toStdString());
        m_ui->txtCommand->setText("");
    }

    void ServerPage::uploadMod()
    {
        QStringList files{ QFileDialog::getOpenFileNames(this, _("Select Mod Files"), {}, _("Jar Files (*.jar)")) };
        for(const QString& file : files)
        {
            m_controller->uploadMod(file.toStdString());
        }
        reloadMods();
    }

    void ServerPage::removeMod()
    {
        if(!m_ui->listMods->selectedItems().empty())
        {
            QMessageBox msgBox{ QMessageBox::Icon::Warning, _("Remove Mods?"), _("Are you sure you want to remove the selected mods from the server? This action is irreversible."), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No, this };
            if(msgBox.exec() == QMessageBox::StandardButton::Yes)
            {
                for(QListWidgetItem* item : m_ui->listMods->selectedItems())
                {
                    m_controller->removeMod(item->text().toStdString());
                }
                reloadMods();
            }
        }
    }

    void ServerPage::onPowerChanged(const ParamEventArgs<PowerStatus>& args)
    {
        m_ui->btnStartStop->setEnabled(true);
        switch(args.getParam())
        {
        case PowerStatus::ErrorStarting:
            m_ui->btnStartStop->setText(_("Start"));
            QMessageBox::critical(this, _("Error"), _("Unable to start the server. Please ensure another server is not already running"));
            break;
        case PowerStatus::ErrorStopping:
            m_ui->btnStartStop->setText(_("Stop"));
            QMessageBox::critical(this, _("Error"), _("Unable to stop the server"));
            break;
        case PowerStatus::Started:
            m_ui->btnStartStop->setText(_("Stop"));
            m_ui->btnBroadcast->setEnabled(true);
            break;
        case PowerStatus::Stopped:
            m_ui->btnStartStop->setText(_("Start"));
            m_ui->btnBroadcast->setEnabled(false);
            break;
        }
    }

    void ServerPage::onAddressChanged(const ParamEventArgs<ServerAddress>& args)
    {
        m_ui->btnBroadcast->setText(_("Broadcast"));
        m_ui->btnBroadcast->setEnabled(m_controller->isRunning());
        if(args.getParam().isEmpty())
        {
            QMessageBox::critical(this, _("Error"), _("Unable to broadcast the server. Please ensure your ngrok token is configured in the app's settings and try again"));
        }
        else
        {
            m_ui->lblUrl->setText(QString::fromStdString(args.getParam().getUrl()));
            m_ui->lblPort->setText(QString::number(args.getParam().getPort()));
        }
    }

    void ServerPage::onConsoleOutputChanged(const ParamEventArgs<std::string>& args)
    {
        m_ui->lblOutput->setText(QString::fromStdString(args.getParam()));
    }

    void ServerPage::onResourceUsageChanged(const ParamEventArgs<std::pair<double, unsigned long long>>& args)
    {
        m_ui->lblCPU->setText(QString::fromStdString(std::format("{}%", args.getParam().first)));
        m_ui->lblRAM->setText(QString::fromStdString(m_controller->getRAMString(args.getParam().second)));
    }

    void ServerPage::reloadMods()
    {
        m_ui->listMods->clear();
        for(const std::string& mod : m_controller->getMods())
        {
            m_ui->listMods->addItem(new QListWidgetItem(QLEMENTINE_ICON(Misc_Blocks), QString::fromStdString(mod), m_ui->listMods));
        }
    }
}
