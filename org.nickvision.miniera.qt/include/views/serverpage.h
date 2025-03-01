#ifndef SERVERPAGE_H
#define SERVERPAGE_H

#include <memory>
#include <QShowEvent>
#include <QWidget>
#include "controllers/serverviewcontroller.h"

namespace Ui { class ServerPage; }

namespace Nickvision::Miniera::Qt::Views
{
    /**
     * @brief A page for managing a server.
     */
    class ServerPage : public QWidget
    {
    Q_OBJECT

    public:
        /**
         * @brief Constructs a ServerPage.
         * @param controller The ServerViewController
         * @param parent The parent widget
         */
        ServerPage(const std::shared_ptr<Shared::Controllers::ServerViewController>& controller, QWidget* parent = nullptr);
        /**
         * @brief Destructs a ServerPage.
         */
        ~ServerPage();

    protected:
        /**
         * @brief Handles when the page is shown.
         * @param event QShowEvent
         */
        void showEvent(QShowEvent* event) override;

    private:
        Ui::ServerPage* m_ui;
        std::shared_ptr<Shared::Controllers::ServerViewController> m_controller;
    };
}

#endif //SERVERPAGE_H
