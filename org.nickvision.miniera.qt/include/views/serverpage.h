#ifndef SERVERPAGE_H
#define SERVERPAGE_H

#include <memory>
#include <QWidget>
#include "controllers/serverviewcontroller.h"
#include "models/boundedqueue.h"

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

    private Q_SLOTS:
        /**
         * @brief Starts or stops the server.
         */
        void startStop();
        /**
         * @brief Broadcasts the server.
         */
        void broadcast();
        /**
         * @brief Sends a command to the server.
         */
        void sendCommand();

    private:
        /**
         * @brief Handles when the console output is changed.
         * @param args ParamEventArgs<std::string>
         */
        void onConsoleOutputChanged(const Events::ParamEventArgs<std::string>& args);
        Ui::ServerPage* m_ui;
        std::shared_ptr<Shared::Controllers::ServerViewController> m_controller;
        Shared::Models::BoundedQueue<double> m_cpuQueue;
        Shared::Models::BoundedQueue<unsigned long long> m_memQueue;
    };
}

#endif //SERVERPAGE_H
