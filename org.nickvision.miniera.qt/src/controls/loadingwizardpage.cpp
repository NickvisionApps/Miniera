#include "controls/loadingwizardpage.h"
#include <QVBoxLayout>
#include <libnick/localization/gettext.h>
#include "controls/statuspage.h"
#include "helpers/qthelpers.h"

using namespace oclero::qlementine;

namespace Nickvision::Miniera::Qt::Controls
{
    LoadingWizardPage::LoadingWizardPage(const QString& title, const QString& subtitle, QWidget* parent)
        : QWizardPage{ parent },
        m_finished{ false }
    {
        m_viewStack = new QStackedWidget(this);
        //Page Settings
        setTitle(title);
        setSubTitle(subtitle);
        //Progress Page
        m_spinner = new LoadingSpinner(this);
        m_spinner->setMinimumSize(48, 48);
        m_spinner->setSpinning(true);
        QWidget* progressPage{ new QWidget(this) };
        QHBoxLayout* layoutSpinner{ new QHBoxLayout() };
        layoutSpinner->addStretch();
        layoutSpinner->addWidget(m_spinner);
        layoutSpinner->addStretch();
        QVBoxLayout* layoutProgress{ new QVBoxLayout() };
        layoutProgress->addStretch();
        layoutProgress->addLayout(layoutSpinner);
        layoutProgress->addStretch();
        progressPage->setLayout(layoutProgress);
        m_viewStack->addWidget(progressPage);
        //Finished Page
        StatusPage* statusPage{ new StatusPage(this) };
        statusPage->setTitle(_("Completed"));
        statusPage->setIcon(QLEMENTINE_ICON(Misc_Success));
        m_viewStack->addWidget(statusPage);
        //Layout
        QVBoxLayout* layout{ new QVBoxLayout() };
        layout->addWidget(m_viewStack);
        setLayout(layout);
        m_viewStack->setCurrentIndex(0);
    }

    bool LoadingWizardPage::isFinished() const
    {
        return m_finished;
    }

    void LoadingWizardPage::setIsFinished(bool finished)
    {
        m_finished = finished;
        m_viewStack->setCurrentIndex(m_finished ? 1 : 0);
        m_spinner->setSpinning(!finished);
        Q_EMIT completeChanged();
    }

    bool LoadingWizardPage::isComplete() const
    {
        return m_finished;
    }
}
