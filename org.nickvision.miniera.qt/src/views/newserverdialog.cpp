#include "views/newserverdialog.h"
#include <QComboBox>
#include <QDesktopServices>
#include <QFormLayout>
#include <QLabel>
#include <QRadioButton>
#include <QSpinBox>
#include <QStyleFactory>
#include <QVBoxLayout>
#include <QWizardPage>
#include <oclero/qlementine/widgets/Switch.hpp>
#include <oclero/qlementine/widgets/LineEdit.hpp>
#include <libnick/localization/gettext.h>

using namespace oclero::qlementine;

enum NewServerDialogPage
{
    Introduction = 0,
    Basic,
    Network,
    Java,
    Bedrock,
    Forge,
    Final
};

namespace Ui
{
    class NewServerDialog
    {
    public:
        void setupUi(Nickvision::Miniera::Qt::Views::NewServerDialog* parent)
        {
            //Introduction Page
            QLabel* lblintro{ new QLabel(_("Select the type of server to create:"), parent) };
            lblintro->setWordWrap(true);
            btnJava = new QRadioButton(_("Java Edition"), parent);
            btnJava->setChecked(true);
            btnBedrock = new QRadioButton(_("Bedrock Edition"), parent);
            btnForge = new QRadioButton(_("Forge Edition (Java Modded)"), parent);
            QVBoxLayout* layoutIntro{ new QVBoxLayout() };
            layoutIntro->addWidget(lblintro);
            layoutIntro->addWidget(btnJava);
            layoutIntro->addWidget(btnBedrock);
            layoutIntro->addWidget(btnForge);
            QWizardPage* introPage{ new QWizardPage(parent) };
            introPage->setTitle(_("Introduction"));
            introPage->setSubTitle(_("This wizard will help you create a new server."));
            introPage->setLayout(layoutIntro);
            parent->setPage(NewServerDialogPage::Introduction, introPage);
            //Basic Page
            txtServerName = new LineEdit(parent);
            txtServerName->setPlaceholderText(_("Enter server name here"));
            txtLevelSeed = new LineEdit(parent);
            txtLevelSeed->setPlaceholderText(_("Enter level seed here"));
            cmbGamemode = new QComboBox(parent);
            cmbGamemode->addItem(_("Survival"));
            cmbGamemode->addItem(_("Creative"));
            cmbGamemode->addItem(_("Adventure"));
            cmbDifficulty = new QComboBox(parent);
            cmbDifficulty->addItem(_("Peaceful"));
            cmbDifficulty->addItem(_("Easy"));
            cmbDifficulty->addItem(_("Normal"));
            cmbDifficulty->addItem(_("Hard"));
            chkForceGamemode = new Switch(parent);
            QVBoxLayout* layoutBasic{ new QVBoxLayout(parent) };
            QFormLayout* formBasic{ new QFormLayout(parent) };
            formBasic->addRow(_("Server Name *"), txtServerName);
            formBasic->addRow(_("Level Seed"), txtLevelSeed);
            formBasic->addRow(_("Gamemode"), cmbGamemode);
            formBasic->addRow(_("Difficulty"), cmbDifficulty);
            formBasic->addRow(_("Force Gamemode"), chkForceGamemode);
            layoutBasic->addLayout(formBasic);
            layoutBasic->addStretch();
            layoutBasic->addWidget(new QLabel(_("* Required"), parent));
            QWizardPage* basicPage{ new QWizardPage(parent) };
            basicPage->setTitle(_("Basic Properties"));
            basicPage->setSubTitle(_("Please configure the basic properties of the server."));
            basicPage->setLayout(layoutBasic);
            parent->setPage(NewServerDialogPage::Basic, basicPage);
            //Network Page
            spnServerPort = new QSpinBox(parent);
            spnServerPort->setMinimum(1);
            spnServerPort->setMaximum(65535);
            spnMaxPlayers = new QSpinBox(parent);
            spnMaxPlayers->setMinimum(1);
            spnMaxPlayers->setMaximum(256);
            spnViewDistance = new QSpinBox(parent);
            spnViewDistance->setMinimum(3);
            spnViewDistance->setMaximum(32);
            spnPlayerIdleTimeout = new QSpinBox(parent);
            chkOnlineMode = new Switch(parent);
            chkOnlineMode->setChecked(true);
            chkOnlineMode->setToolTip(_("Online Mode"));
            QVBoxLayout* layoutNetwork{ new QVBoxLayout(parent) };
            QFormLayout* formNetwork{ new QFormLayout(parent) };
            formNetwork->addRow(_("Server Port *"), spnServerPort);
            formNetwork->addRow(_("Maximum Connected Players"), spnMaxPlayers);
            formNetwork->addRow(_("View Distance *"), spnViewDistance);
            formNetwork->addRow(_("Player Idle Minutes Until Kicked"), spnPlayerIdleTimeout);
            formNetwork->addRow(_("Validate Users *"), chkOnlineMode);
            layoutNetwork->addLayout(formNetwork);
            layoutNetwork->addStretch();
            layoutNetwork->addWidget(new QLabel(_("* Default Recommended"), parent));
            QWizardPage* networkPage{ new QWizardPage(parent) };
            networkPage->setTitle(_("Network Properties"));
            networkPage->setSubTitle(_("Please configure the network properties of the server."));
            networkPage->setLayout(layoutNetwork);
            parent->setPage(NewServerDialogPage::Network, networkPage);
            //Java Page
            chkAllowFlight = new Switch(parent);
            chkAllowNether = new Switch(parent);
            chkEnableCommandBlock = new Switch(parent);
            chkGenerateStructures = new Switch(parent);
            chkHardcore = new Switch(parent);
            chkPVP = new Switch(parent);
            chkSpawnAnimals = new Switch(parent);
            chkSpawnMonsters = new Switch(parent);
            chkSpawnNPCs = new Switch(parent);
            QFormLayout* formJava{ new QFormLayout(parent) };
            formJava->addRow(_("Allow Flying"), chkAllowFlight);
            formJava->addRow(_("Allow Nether"), chkAllowNether);
            formJava->addRow(_("Enable Command Blocks"), chkEnableCommandBlock);
            formJava->addRow(_("Generate Structures"), chkGenerateStructures);
            formJava->addRow(_("Hardcore Mode"), chkHardcore);
            formJava->addRow(_("PVP"), chkPVP);
            formJava->addRow(_("Spawn Animals"), chkSpawnAnimals);
            formJava->addRow(_("Spawn Monsters"), chkSpawnMonsters);
            formJava->addRow(_("Spawn NPCs"), chkSpawnNPCs);
            QWizardPage* javaPage{ new QWizardPage(parent) };
            javaPage->setTitle(_("Java Properties"));
            javaPage->setSubTitle(_("Please configure the properties of the java server."));
            javaPage->setLayout(formJava);
            parent->setPage(NewServerDialogPage::Java, javaPage);
            //Bedrock Page
            chkAllowCheats = new Switch(parent);
            chkClientSideChunkGenerationEnabled = new Switch(parent);
            chkDisableCustomSkins = new Switch(parent);
            spnTickDistance = new QSpinBox(parent);
            spnTickDistance->setMinimum(4);
            spnTickDistance->setMaximum(12);
            QVBoxLayout* layoutBedrock{ new QVBoxLayout(parent) };
            QFormLayout* formBedrock{ new QFormLayout(parent) };
            formBedrock->addRow(_("Allow Cheats"), chkAllowCheats);
            formBedrock->addRow(_("Client-Side Chunk Generation *"), chkClientSideChunkGenerationEnabled);
            formBedrock->addRow(_("Disable Custom Skins"), chkDisableCustomSkins);
            formBedrock->addRow(_("Tick Distance *"), spnTickDistance);
            layoutBedrock->addLayout(formBedrock);
            layoutBedrock->addStretch();
            layoutBedrock->addWidget(new QLabel(_("* Default Recommended"), parent));
            QWizardPage* bedrockPage{ new QWizardPage(parent) };
            bedrockPage->setTitle(_("Bedrock Properties"));
            bedrockPage->setSubTitle(_("Please configure the properties of the bedrock server."));
            bedrockPage->setLayout(layoutBedrock);
            parent->setPage(NewServerDialogPage::Bedrock, bedrockPage);
            //Forge Page
            QFormLayout* formForge{ new QFormLayout(parent) };
            QWizardPage* forgePage{ new QWizardPage(parent) };
            forgePage->setTitle(_("Forge Properties"));
            forgePage->setSubTitle(_("Please configure the properties of the forge server."));
            forgePage->setLayout(formForge);
            parent->setPage(NewServerDialogPage::Forge, forgePage);
            //Final Page
            QLabel* lblFinal{ new QLabel(parent) };
            lblFinal->setText(_("Once the server is created, you will not be able to change some of these settings."));
            lblFinal->setWordWrap(true);
            QLabel* lblEula{ new QLabel(parent) };
            lblEula->setText(_("By clicking 'Finish', you agree to the following EULA: <a href=\"https://account.mojang.com/documents/minecraft_eula\">https://account.mojang.com/documents/minecraft_eula</a>"));
            lblEula->setTextFormat(Qt::RichText);
            lblEula->setTextInteractionFlags(Qt::TextBrowserInteraction);
            lblEula->setOpenExternalLinks(true);
            lblEula->setWordWrap(true);
            QVBoxLayout* layoutFinal{ new QVBoxLayout(parent) };
            layoutFinal->addWidget(lblFinal);
            layoutFinal->addStretch();
            layoutFinal->addWidget(lblEula);
            QWizardPage* finalPage{ new QWizardPage(parent) };
            finalPage->setTitle(_("Complete Setup"));
            finalPage->setSubTitle(_("Please ensure all settings are correct before creating the server."));
            finalPage->setLayout(layoutFinal);
            parent->setPage(NewServerDialogPage::Final, finalPage);
        }

        QRadioButton* btnJava;
        QRadioButton* btnBedrock;
        QRadioButton* btnForge;
        LineEdit* txtServerName;
        LineEdit* txtLevelSeed;
        QComboBox* cmbGamemode;
        QComboBox* cmbDifficulty;
        Switch* chkForceGamemode;
        QSpinBox* spnServerPort;
        QSpinBox* spnMaxPlayers;
        QSpinBox* spnViewDistance;
        QSpinBox* spnPlayerIdleTimeout;
        Switch* chkOnlineMode;
        Switch* chkAllowFlight;
        Switch* chkAllowNether;
        Switch* chkEnableCommandBlock;
        Switch* chkGenerateStructures;
        Switch* chkHardcore;
        Switch* chkPVP;
        Switch* chkSpawnAnimals;
        Switch* chkSpawnMonsters;
        Switch* chkSpawnNPCs;
        Switch* chkAllowCheats;
        Switch* chkClientSideChunkGenerationEnabled;
        Switch* chkDisableCustomSkins;
        QSpinBox* spnTickDistance;
    };
}

namespace Nickvision::Miniera::Qt::Views
{
    NewServerDialog::NewServerDialog(QWidget* parent)
        : QWizard{ parent },
        m_ui{ new Ui::NewServerDialog() }
    {
        //Window Settings
        setWindowTitle(_("New Server"));
        setMinimumSize(600, 400);
        setModal(true);
        setWizardStyle(QWizard::ModernStyle);
        setOption(QWizard::HaveHelpButton, true);
        //Load Ui
        m_ui->setupUi(this);
        //Signals
        connect(this, &QWizard::helpRequested, this, &NewServerDialog::help);
    }

    NewServerDialog::~NewServerDialog()
    {
        delete m_ui;
    }

    int NewServerDialog::nextId() const
    {
        switch(currentId())
        {
        case NewServerDialogPage::Introduction:
            return NewServerDialogPage::Basic;
        case NewServerDialogPage::Basic:
            return NewServerDialogPage::Network;
        case NewServerDialogPage::Network:
            if(m_ui->btnJava->isChecked())
            {
                return NewServerDialogPage::Java;
            }
            else if(m_ui->btnBedrock->isChecked())
            {
                return NewServerDialogPage::Bedrock;
            }
            else if(m_ui->btnForge->isChecked())
            {
                return NewServerDialogPage::Java;
            }
            break;
        case NewServerDialogPage::Java:
            if(m_ui->btnForge->isChecked())
            {
                return NewServerDialogPage::Forge;
            }
            else
            {
                return NewServerDialogPage::Final;
            }
            break;
        case NewServerDialogPage::Bedrock:
        case NewServerDialogPage::Forge:
            return NewServerDialogPage::Final;
        }
        return -1;
    }

    void NewServerDialog::help()
    {
        QDesktopServices::openUrl(QUrl("https://minecraft.fandom.com/wiki/Server.properties"));
    }
}