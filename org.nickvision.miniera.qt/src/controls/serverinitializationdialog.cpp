#include "controls/serverinitializationdialog.h"
#include <chrono>
#include <format>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QScrollArea>
#include <QTimer>
#include <QVBoxLayout>
#include <libnick/localization/gettext.h>
#include <oclero/qlementine/widgets/StatusBadgeWidget.hpp>

using namespace Nickvision::Miniera::Shared::Events;
using namespace oclero::qlementine;

namespace Ui
{
    class ServerInitializationDialog
    {
    public:
        void setupUi(Nickvision::Miniera::Qt::Controls::ServerInitializationDialog* parent)
        {
            QFont boldFont;
            boldFont.setBold(true);
            //Status Icon
            status = new StatusBadgeWidget(parent);
            //Title Label
            lblTitle = new QLabel(parent);
            lblTitle->setFont(boldFont);
            //Wait Label
            QLabel* lblWait{ new QLabel(parent) };
            lblWait->setText(_("This may take some time."));
            //Progress Bar
            progBar = new QProgressBar(parent);
            progBar->setTextVisible(false);
            //Log Label
            lblLog = new QLabel(parent);
            lblLog->setAlignment(::Qt::AlignTop);
            lblLog->setWordWrap(true);
            //Layout
            QHBoxLayout* layoutHeader{ new QHBoxLayout() };
            layoutHeader->addWidget(status);
            layoutHeader->addWidget(lblTitle);
            QScrollArea* scroll{ new QScrollArea(parent) };
            scroll->setVerticalScrollBarPolicy(::Qt::ScrollBarAsNeeded);
            scroll->setHorizontalScrollBarPolicy(::Qt::ScrollBarAlwaysOff);
            scroll->setWidgetResizable(true);
            scroll->setWidget(lblLog);
            QVBoxLayout* layoutMain{ new QVBoxLayout() };
            layoutMain->addLayout(layoutHeader);
            layoutMain->addWidget(lblWait);
            layoutMain->addWidget(progBar);
            layoutMain->addWidget(scroll);
            parent->setLayout(layoutMain);
        }

        StatusBadgeWidget* status;
        QLabel* lblTitle;
        QProgressBar* progBar;
        QLabel* lblLog;
    };
}

namespace Nickvision::Miniera::Qt::Controls
{
    ServerInitializationDialog::ServerInitializationDialog(QWidget* parent)
        : QDialog{ parent },
        m_ui{ new Ui::ServerInitializationDialog() },
        m_flags{ windowFlags() }
    {
        //Window Settings
        setWindowTitle(_("Server Initialization"));
        setMinimumSize(500, 300);
        setModal(true);
        //Load Ui
        m_ui->setupUi(this);
    }

    ServerInitializationDialog::~ServerInitializationDialog()
    {
        delete m_ui;
    }

    void ServerInitializationDialog::updateProgress(const ServerInitializationProgressChangedEventArgs& args)
    {
        setWindowFlags(m_flags & ~::Qt::WindowCloseButtonHint);
        m_ui->progBar->setMinimum(0);
        m_ui->progBar->setMaximum(100);
        m_ui->progBar->setValue(static_cast<int>(args.getProgress() * 100));
        m_ui->lblLog->setText(QString::fromStdString(args.getLog()));
        if(args.isFinished())
        {
            if(args.isError())
            {
                setWindowFlags(m_flags);
                m_ui->status->setBadge(StatusBadge::Error);
                m_ui->lblTitle->setText(QString::fromStdString(std::vformat(_("({}) Failed to initialize server."), std::make_format_args(args.getName()))));
            }
            else
            {
                m_ui->status->setBadge(StatusBadge::Success);
                m_ui->lblTitle->setText(QString::fromStdString(std::vformat(_("({}) Initialized server."), std::make_format_args(args.getName()))));
                QTimer::singleShot(std::chrono::milliseconds(500), [this]()
                {
                    close();
                });
                return;
            }
        }
        else
        {
            m_ui->status->setBadge(StatusBadge::Info);
            m_ui->lblTitle->setText(QString::fromStdString(std::vformat(_("({}) Initializing server..."), std::make_format_args(args.getName()))));
            if(args.getProgress() == 0)
            {
                m_ui->progBar->setMaximum(0);
                m_ui->progBar->setValue(-1);
            }
        }
        show();
    }
}
