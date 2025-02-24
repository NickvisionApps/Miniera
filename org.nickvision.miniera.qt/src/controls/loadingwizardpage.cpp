#include "controls/loadingwizardpage.h"
#include <QProgressBar>
#include <QVBoxLayout>
#include <libnick/localization/gettext.h>
#include "controls/statuspage.h"
#include "helpers/qthelpers.h"

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
        m_prgLoading = new QProgressBar(this);
        m_prgLoading->setRange(0, 0);
        m_prgLoading->setValue(-1);
        m_prgLoading->setAlignment(::Qt::AlignCenter);
        QWidget* progressPage{ new QWidget(this) };
        QVBoxLayout* layoutProgress{ new QVBoxLayout() };
        layoutProgress->addStretch();
        layoutProgress->addWidget(m_prgLoading);
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
        m_prgLoading->setValue(-1);
        Q_EMIT completeChanged();
    }

    bool LoadingWizardPage::isComplete() const
    {
        return m_finished;
    }
}