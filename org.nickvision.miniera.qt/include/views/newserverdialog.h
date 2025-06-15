#ifndef NEWSERVERDIALOG_H
#define NEWSERVERDIALOG_H

#include <memory>
#include <QCloseEvent>
#include <QWizard>
#include "controllers/newserverdialogcontroller.h"

namespace Ui { class NewServerDialog; }

namespace Nickvision::Miniera::Qt::Views
{
    /**
     * @brief A dialog for creating a new server.
     */
    class NewServerDialog : public QWizard
    {
    Q_OBJECT

    public:
        /**
         * @brief Constructs a NewServerDialog.
         * @param controller The NewServerDialogController
         * @param parent The parent widget
         */
        NewServerDialog(const std::shared_ptr<Shared::Controllers::NewServerDialogController>& controller, QWidget* parent = nullptr);
        /**
         * @brief Destructs a NewServerDialog.
         */
        ~NewServerDialog();
        /**
         * @brief Gets the next page id.
         * @return The next page id
         */
        int nextId() const override;

    private Q_SLOTS:
        /**
         * @brief Opens the server properties help website.
         */
        void help();
        /**
         * @brief Finishes the dialog.
         */
        void finish();
    
    private:
        /**
         * @brief Handles when the server versions are loaded.
         * @param args ServerVersionsLoadedEventArgs
         */
        void onServerVersionsLoaded(const Shared::Events::ServerVersionsLoadedEventArgs& args);
        /**
         * @brief Loads the UI from the selected edition.
         * @param edition The selected edition
         */
        void loadUiFromEdition(Shared::Models::Edition edition) const;
        Ui::NewServerDialog* m_ui;
        std::shared_ptr<Shared::Controllers::NewServerDialogController> m_controller;
    };
}

#endif //NEWSERVERDIALOG_H