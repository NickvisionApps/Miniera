#include "views/serverpage.h"
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <libnick/localization/gettext.h>
#include <oclero/qlementine/widgets/ActionButton.hpp>
#include <oclero/qlementine/widgets/SegmentedControl.hpp>
#include "helpers/qthelpers.h"

using namespace Nickvision::Miniera::Qt::Helpers;
using namespace Nickvision::Miniera::Shared::Controllers;
using namespace oclero::qlementine;

namespace Ui
{
    class ServerPage
    {
    public:
        void setupUi(Nickvision::Miniera::Qt::Views::ServerPage* parent)
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
            btnRestart = new ActionButton(parent);
            btnRestart->setAutoDefault(false);
            btnRestart->setDefault(false);
            btnRestart->setMinimumWidth(200);
            btnRestart->setText(_("Restart"));
            btnRestart->setIcon(QLEMENTINE_ICON(Action_Refresh));
            QVBoxLayout* layoutInfo{ new QVBoxLayout() };
            layoutInfo->addWidget(lblName);
            layoutInfo->addWidget(lblVersion);
            QVBoxLayout* layoutActions{ new QVBoxLayout() };
            layoutActions->addWidget(btnStartStop);
            layoutActions->addWidget(btnRestart);
            QHBoxLayout* layoutHeader{ new QHBoxLayout() };
            layoutHeader->addLayout(layoutInfo);
            layoutHeader->addStretch();
            layoutHeader->addLayout(layoutActions);
            //Navigation
            navBar = new SegmentedControl(parent);
            navBar->addItem(_("Dashboard"), QLEMENTINE_ICON(Navigation_UiPanelTop));
            navBar->addItem(_("Console"), QLEMENTINE_ICON(Software_CommandLine));
            navBar->addItem(_("Settings"), QLEMENTINE_ICON(Navigation_Settings));
            QHBoxLayout* layoutNavigation{ new QHBoxLayout() };
            layoutNavigation->addWidget(navBar);
            layoutNavigation->addStretch();
            //Main Layout
            QFrame* line{ new QFrame(parent) };
            line->setFrameShape(QFrame::Shape::HLine);
            line->setFrameShadow(QFrame::Shadow::Sunken);
            QVBoxLayout* layoutMain{ new QVBoxLayout(parent) };
            layoutMain->addLayout(layoutHeader);
            layoutMain->addWidget(line);
            layoutMain->addLayout(layoutNavigation);
            layoutMain->addStretch();
            parent->setLayout(layoutMain);
        }

        QLabel* lblName;
        QLabel* lblVersion;
        ActionButton* btnStartStop;
        ActionButton* btnRestart;
        SegmentedControl* navBar;
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
        m_ui->setupUi(this);
        m_ui->lblName->setText(QString::fromStdString(m_controller->getName()));
        m_ui->lblVersion->setText(QString::fromStdString(m_controller->getVersion()));
    }

    ServerPage::~ServerPage()
    {
        delete m_ui;
    }

    void ServerPage::show()
    {
        QWidget::show();
    }
}