#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <adwaita.h>
#include "controllers/mainwindowcontroller.h"
#include "helpers/builder.h"

namespace Nickvision::Miniera::GNOME::Views
{
    /**
     * @brief The main window for the application. 
     */
    class MainWindow
    {
    public:
        /**
         * @brief Constructs a MainWindow.
         * @param controller The MainWindowController
         * @param app The GtkApplication object of the running app 
         */
        MainWindow(const std::shared_ptr<Shared::Controllers::MainWindowController>& controller, GtkApplication* app);
        /**
         * @brief Destructs the MainWindow. 
         */
        ~MainWindow();
        /**
         * @brief Shows the main window. 
         */
        void show();

    private:
        /**
         * @brief Handles when the window requests to close.
         * @return True to prevent closing, else false 
         */
        bool onCloseRequested();
        /**
         * @brief Handles when a notification is sent to the window.
         * @param args Nickvision::Notifications::NotificationSentEventArgs 
         */
        void onNotificationSent(const Nickvision::Notifications::NotificationSentEventArgs& args);
        /**
         * @brief Quits the application. 
         */
        void quit();
        /**
         * @brief Opens the application's preferences dialog. 
         */
        void preferences();
        /**
         * @brief Opens the application's keyboard shortcut dialog.
         */
        void keyboardShortcuts();
        /**
         * @brief Opens the application's about dialog. 
         */
        void about();
        std::shared_ptr<Shared::Controllers::MainWindowController> m_controller;
        GtkApplication* m_app;
        Helpers::Builder m_builder;
        AdwApplicationWindow* m_window;
    };
}

#endif //MAINWINDOW_H
