#ifndef NEWSERVERDIALOG_H
#define NEWSERVERDIALOG_H

#include <QWizard>

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
         * @param parent The parent widget
         */
        NewServerDialog(QWidget* parent = nullptr);
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
    
    private:
        Ui::NewServerDialog* m_ui;
    };
}

#endif //NEWSERVERDIALOG_H