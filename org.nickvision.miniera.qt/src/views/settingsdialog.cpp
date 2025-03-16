#include "views/settingsdialog.h"
#include <QApplication>
#include <QComboBox>
#include <QDesktopServices>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStyleHints>
#include <libnick/localization/gettext.h>
#include <oclero/qlementine/widgets/Switch.hpp>
#include <oclero/qlementine/widgets/LineEdit.hpp>
#include "helpers/qthelpers.h"

using namespace Nickvision::Miniera::Shared::Controllers;
using namespace oclero::qlementine;

namespace Ui 
{
    class SettingsDialog
    {
    public:
        void setupUi(Nickvision::Miniera::Qt::Views::SettingsDialog* parent)
        {
            viewStack = new QStackedWidget(parent);
            //User Interface Page
            QLabel* lblTheme{ new QLabel(parent) };
            lblTheme->setText(_("Theme"));
            cmbTheme = new QComboBox(parent);
            cmbTheme->addItem(_("Light"));
            cmbTheme->addItem(_("Dark"));
            cmbTheme->addItem(_("System"));
            QLabel* lblUpdates{ new QLabel(parent) };
            lblUpdates->setText(_("Automatically Check for Updates"));
            chkUpdates = new Switch(parent);
            QFormLayout* layoutUserInterface{ new QFormLayout() };
            layoutUserInterface->addRow(lblTheme, cmbTheme);
            layoutUserInterface->addRow(lblUpdates, chkUpdates);
            QWidget* userInterfacePage{ new QWidget(parent) };
            userInterfacePage->setLayout(layoutUserInterface);
            viewStack->addWidget(userInterfacePage);
            //Server Page
            QLabel* lblServerRam{ new QLabel(parent) };
            lblServerRam->setText(_("Maximum Server RAM (GB)"));
            spnServerRam = new QSpinBox(parent);
            spnServerRam->setMinimum(1);
            QLabel* lblBroadcastOnStart{ new QLabel(parent) };
            lblBroadcastOnStart->setText(_("Broadcast on Start"));
            chkBroadcastOnStart = new Switch(parent);
            QFormLayout* layoutServer{ new QFormLayout() };
            layoutServer->addRow(lblServerRam, spnServerRam);
            layoutServer->addRow(lblBroadcastOnStart, chkBroadcastOnStart);
            QWidget* serverPage{ new QWidget(parent) };
            serverPage->setLayout(layoutServer);
            viewStack->addWidget(serverPage);
            //Hosting Page
            QLabel* lblNgrokAuthToken{ new QLabel(parent) };
            lblNgrokAuthToken->setText(_("Ngrok Auth Token"));
            txtNgrokAuthToken = new LineEdit(parent);
            txtNgrokAuthToken->setPlaceholderText(_("Enter ngrok auth token here"));
            btnNgrokAuthToken = new QPushButton(parent);
            btnNgrokAuthToken->setAutoDefault(false);
            btnNgrokAuthToken->setText(_("Retrieve Auth Token"));
            btnNgrokAuthToken->setIcon(QLEMENTINE_ICON(Navigation_Search));
            QFormLayout* layoutHosting{ new QFormLayout() };
            layoutHosting->addRow(lblNgrokAuthToken, txtNgrokAuthToken);
            layoutHosting->addRow(nullptr, btnNgrokAuthToken);
            QWidget* hostingPage{ new QWidget(parent) };
            hostingPage->setLayout(layoutHosting);
            viewStack->addWidget(hostingPage);
            //Navigation List
            listNavigation = new QListWidget(parent);
            listNavigation->setMaximumWidth(160);
            listNavigation->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
            listNavigation->setDropIndicatorShown(false);
            listNavigation->addItem(new QListWidgetItem(QLEMENTINE_ICON(Navigation_UiPanelLeft), _("User Interface"), listNavigation));
            listNavigation->addItem(new QListWidgetItem(QLEMENTINE_ICON(Hardware_Server), _("Server"), listNavigation));
            listNavigation->addItem(new QListWidgetItem(QLEMENTINE_ICON(Misc_Globe), _("Hosting"), listNavigation));
            QObject::connect(listNavigation, &QListWidget::currentRowChanged, [this]()
            {
                viewStack->setCurrentIndex(listNavigation->currentRow());
            });
            //Main Layout
            QHBoxLayout* layout{ new QHBoxLayout() };
            layout->addWidget(listNavigation);
            layout->addWidget(viewStack);
            parent->setLayout(layout);
        }

        QListWidget* listNavigation;
        QStackedWidget* viewStack;
        QComboBox* cmbTheme;
        Switch* chkUpdates;
        QSpinBox* spnServerRam;
        Switch* chkBroadcastOnStart;
        LineEdit* txtNgrokAuthToken;
        QPushButton* btnNgrokAuthToken;
    };
}

namespace Nickvision::Miniera::Qt::Views
{
    SettingsDialog::SettingsDialog(const std::shared_ptr<PreferencesViewController>& controller, oclero::qlementine::ThemeManager* themeManager, QWidget* parent)
        : QDialog{ parent },
        m_ui{ new Ui::SettingsDialog() },
        m_controller{ controller },
        m_themeManager{ themeManager }
    {
        //Dialog Settings
        setWindowTitle(_("Settings"));
        setMinimumSize(600, 400);
        setModal(true);
        //Load Ui
        m_ui->setupUi(this);
        m_ui->cmbTheme->setCurrentIndex(static_cast<int>(m_controller->getTheme()));
        m_ui->chkUpdates->setChecked(m_controller->getAutomaticallyCheckForUpdates());
        m_ui->spnServerRam->setMaximum(static_cast<int>(m_controller->getSystemRamInGB()));
        m_ui->spnServerRam->setValue(static_cast<int>(m_controller->getMaxServerRamInGB()));
        m_ui->chkBroadcastOnStart->setChecked(m_controller->getBroadcastOnStart());
        m_ui->txtNgrokAuthToken->setText(QString::fromStdString(m_controller->getNgrokAuthToken()));
        m_ui->listNavigation->setCurrentRow(0);
        //Signals
        connect(m_ui->cmbTheme, &QComboBox::currentIndexChanged, this, &SettingsDialog::onThemeChanged);
        connect(m_ui->btnNgrokAuthToken, &QPushButton::clicked, this, &SettingsDialog::retrieveNgrokAuthToken);
    }

    SettingsDialog::~SettingsDialog()
    {
        delete m_ui;
    }

    void SettingsDialog::closeEvent(QCloseEvent* event)
    {
        m_controller->setTheme(static_cast<Shared::Models::Theme>(m_ui->cmbTheme->currentIndex()));
        m_controller->setAutomaticallyCheckForUpdates(m_ui->chkUpdates->isChecked());
        m_controller->setMaxServerRamInGB(static_cast<unsigned int>(m_ui->spnServerRam->value()));
        m_controller->setBroadcastOnStart(m_ui->chkBroadcastOnStart->isChecked());
        m_controller->setNgrokAuthToken(m_ui->txtNgrokAuthToken->text().toStdString());
        m_controller->saveConfiguration();
        event->accept();
    }

    void SettingsDialog::onThemeChanged()
    {
        switch (static_cast<Shared::Models::Theme>(m_ui->cmbTheme->currentIndex()))
        {
        case Shared::Models::Theme::Light:
            QApplication::styleHints()->setColorScheme(::Qt::ColorScheme::Light);
            m_themeManager->setCurrentTheme("Light");
            break;
        case Shared::Models::Theme::Dark:
            QApplication::styleHints()->setColorScheme(::Qt::ColorScheme::Dark);
            m_themeManager->setCurrentTheme("Dark");
            break;
        default:
            QApplication::styleHints()->unsetColorScheme();
            m_themeManager->setCurrentTheme(QApplication::styleHints()->colorScheme() == ::Qt::ColorScheme::Light ? "Light" : "Dark");
            break;
        }
    }

    void SettingsDialog::retrieveNgrokAuthToken()
    {
        QDesktopServices::openUrl(QUrl(QString::fromStdString(m_controller->getNgrokAuthTokenRetrievalUrl())));
    }
}
