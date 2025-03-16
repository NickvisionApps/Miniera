#ifndef SERVERPAGE_H
#define SERVERPAGE_H

#include <memory>
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

    public Q_SLOTS:
        /**
         * @brief Starts or stops the server.
         */
        void startStop();
        /**
         * @brief Broadcasts the server.
         */
        void broadcast();

    private Q_SLOTS:
        /**
         * @brief Sends a command to the server.
         */
        void sendCommand();

    private:
        /**
         * @brief Handles when the power is changed.
         * @param args ParamEventArgs<PowerStatus>
         */
        void onPowerChanged(const Events::ParamEventArgs<Shared::Models::PowerStatus>& args);
        /**
         * @brief Handles when the address is changed.
         * @param args ParamEventArgs<ServerAddress>
         */
        void onAddressChanged(const Events::ParamEventArgs<Shared::Models::ServerAddress>& args);
        /**
         * @brief Handles when the console output is changed.
         * @param args ParamEventArgs<std::string>
         */
        void onConsoleOutputChanged(const Events::ParamEventArgs<std::string>& args);
        /**
         * @brief Handles when the resource usagae is changed.
         * @param args ParamEventArgs<std::pair<double, unsigned long long>>
         */
        void onResourceUsageChanged(const Events::ParamEventArgs<std::pair<double, unsigned long long>>& args);
        Ui::ServerPage* m_ui;
        std::shared_ptr<Shared::Controllers::ServerViewController> m_controller;
    };
}

#endif //SERVERPAGE_H
