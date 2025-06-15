#ifndef LOADINGWIZARDPAGE_H
#define LOADINGWIZARDPAGE_H

#include <QStackedWidget>
#include <QString>
#include <QWizardPage>
#include <oclero/qlementine/widgets/LoadingSpinner.hpp>

namespace Nickvision::Miniera::Qt::Controls
{
    /**
     * @brief A wizard page that displays a loading bar.
     */
    class LoadingWizardPage : public QWizardPage
    {
    public:
        /**
         * @brief Constructs a LoadingWizardPage.
         * @param title The title of the page
         * @param subtitle The subtitle of the page
         * @param parent The parent widget
         */
        LoadingWizardPage(const QString& title, const QString& subtitle, QWidget* parent);
        /**
         * @brief Destructs a LoadingWizardPage.
         */
        ~LoadingWizardPage() override = default;
        /**
         * @brief Gets if the loading has finished.
         * @return If the loading has finished
         */
        bool isFinished() const;
        /**
         * @brief Sets if the loading has finished.
         * @param finished If the loading has finished
         */
        void setIsFinished(bool finished);
        /**
         * @brief Gets if the page is complete.
         * @return If the page is complete
         */
        bool isComplete() const override;

    private:
        bool m_finished;
        QStackedWidget* m_viewStack;
        oclero::qlementine::LoadingSpinner* m_spinner;
    };
}

#endif //LOADINGWIZARDPAGE_H
