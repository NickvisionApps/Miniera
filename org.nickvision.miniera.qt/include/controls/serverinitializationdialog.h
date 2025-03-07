#ifndef SERVERINITIALIZATIONDIALOG_H
#define SERVERINITIALIZATIONDIALOG_H

#include <QDialog>
#include "events/serverinitializationprogresschangedeventargs.h"

namespace Ui { class ServerInitializationDialog; }

namespace Nickvision::Miniera::Qt::Controls
{
    /**
     * @brief A dialog for showing server initialization.
     */
    class ServerInitializationDialog : public QDialog
    {
    public:
        /**
         * @brief Constructs a ServerInitializationDialog.
         * @param parent The parent widget
         */
        ServerInitializationDialog(QWidget* parent = nullptr);
        /**
         * @brief Destructs a ServerInitializationDialog.
         */
        ~ServerInitializationDialog();
        /**
         * @brief Updates the progress shown in the dialog.
         * @param args ServerInitializationProgressChangedEventArgs
         */
        void updateProgress(const Shared::Events::ServerInitializationProgressChangedEventArgs& args);

    private:
        Ui::ServerInitializationDialog* m_ui;
        ::Qt::WindowFlags m_flags;
    };
}

#endif //SERVERINITIALIZATIONDIALOG_H
