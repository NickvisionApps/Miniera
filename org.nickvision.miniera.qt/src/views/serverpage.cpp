#include "views/serverpage.h"
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <libnick/localization/gettext.h>
#include <oclero/qlementine/widgets/ActionButton.hpp>
#include <oclero/qlementine/widgets/LineEdit.hpp>
#include <oclero/qlementine/widgets/SegmentedControl.hpp>
#include "helpers/qthelpers.h"

using namespace Nickvision::Events;
using namespace Nickvision::Miniera::Qt::Helpers;
using namespace Nickvision::Miniera::Shared::Controllers;
using namespace Nickvision::Miniera::Shared::Models;
using namespace oclero::qlementine;

namespace Ui
{
    class ServerPage
    {
    public:
        void setupUi(Nickvision::Miniera::Qt::Views::ServerPage* parent, bool supportsMods)
        {
            QFont boldFont;
            boldFont.setBold(true);
            boldFont.setPointSize(12);
            //Header
            lblName = new QLabel(parent);
            lblName->setFont(boldFont);
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
            navBar->addItem(_("Console"), QLEMENTINE_ICON(Software_CommandLine));
            navBar->addItem(_("Settings"), QLEMENTINE_ICON(Navigation_Settings));
            if(supportsMods)
            {
                navBar->addItem(_("Mods"), QLEMENTINE_ICON(Misc_Blocks));
            }
            QHBoxLayout* layoutNavigation{ new QHBoxLayout() };
            layoutNavigation->addStretch();
            layoutNavigation->addWidget(navBar);
            layoutNavigation->addStretch();
            //Dashboard Page
            QWidget* pageDashboard = new QWidget(parent);
            //Console Page
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
            scrollConsole->setWidgetResizable(true);
            scrollConsole->setWidget(lblOutput);
            QObject::connect(scrollConsole->verticalScrollBar(), &QScrollBar::rangeChanged, [this, scrollConsole](int, int max)
            {
                scrollConsole->verticalScrollBar()->setValue(max);
            });
            QVBoxLayout* layoutConsole{ new QVBoxLayout() };
            layoutConsole->setContentsMargins(0, 0, 0, 0);
            layoutConsole->addWidget(scrollConsole);
            layoutConsole->addWidget(txtCommand);
            QWidget* pageConsole{ new QWidget(parent) };
            pageConsole->setLayout(layoutConsole);
            //Settings Page
            QWidget* pageSettings{ new QWidget(parent) };
            //Mods Page
            QWidget* pageMods{ new QWidget(parent) };
            //View Stack
            viewStack = new QStackedWidget(parent);
            viewStack->addWidget(pageDashboard);
            viewStack->addWidget(pageConsole);
            viewStack->addWidget(pageSettings);
            viewStack->addWidget(pageMods);
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
        QLabel* lblOutput;
        LineEdit* txtCommand;
    };
}

namespace Nickvision::Miniera::Qt::Views
{
    ServerPage::ServerPage(const std::shared_ptr<ServerViewController>& controller, QWidget* parent)
        : QWidget{ parent },
        m_ui{ new Ui::ServerPage() },
        m_controller{ controller },
        m_cpuQueue{ 5 },
        m_memQueue{ 5 }
    {
        //Load Ui
        m_ui->setupUi(this, m_controller->supportsMods());
        m_ui->lblName->setText(QString::fromStdString(m_controller->getName()));
        m_ui->lblVersion->setText(QString::fromStdString(m_controller->getVersion()));
        //Signals
        m_controller->consoleOutputChanged() += [&](const ParamEventArgs<std::string>& args) { QtHelpers::dispatchToMainThread([this, args]() { onConsoleOutputChanged(args); }); };
        connect(m_ui->btnStartStop, &QPushButton::clicked, this, &ServerPage::startStop);
        connect(m_ui->btnBroadcast, &QPushButton::clicked, this, &ServerPage::broadcast);
        connect(m_ui->txtCommand, &QLineEdit::returnPressed, this, &ServerPage::sendCommand);
    }

    ServerPage::~ServerPage()
    {
        delete m_ui;
    }

    void ServerPage::startStop()
    {
        PowerStatus status{ m_controller->startStop() };
        if(status == PowerStatus::ErrorStarting)
        {
            QMessageBox::critical(this, _("Error"), _("Unable to start the server. Please ensure another server is not already running"));
        }
        else if(status == PowerStatus::ErrorStopping)
        {
            QMessageBox::critical(this, _("Error"), _("Unable to stop the server"));
        }
        else if(status == PowerStatus::Started)
        {
            m_ui->btnStartStop->setText(_("Stop"));
            m_ui->btnBroadcast->setEnabled(true);
        }
        else if(status == PowerStatus::Stopped)
        {
            m_ui->btnStartStop->setText(_("Start"));
            m_ui->btnBroadcast->setEnabled(false);
            m_ui->lblOutput->setText(_("No console output"));
        }
    }

    void ServerPage::broadcast()
    {

    }

    void ServerPage::sendCommand()
    {
        m_controller->sendCommand(m_ui->txtCommand->text().toStdString());
        m_ui->txtCommand->setText("");
    }

    void ServerPage::onConsoleOutputChanged(const ParamEventArgs<std::string>& args)
    {
        m_ui->lblOutput->setText(QString::fromStdString(args.getParam()));
    }
}
