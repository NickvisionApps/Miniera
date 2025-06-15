#ifndef SERVERINITIALIZATIONPROGRESSCHANGEDEVENTARGS_H
#define SERVERINITIALIZATIONPROGRESSCHANGEDEVENTARGS_H

#include <string>
#include <libnick/events/eventargs.h>

namespace Nickvision::Miniera::Shared::Events
{
    /**
     * @brief Event args for when a server initialization's progress is changed.
     */
    class ServerInitializationProgressChangedEventArgs : public Nickvision::Events::EventArgs
    {
    public:
        /**
         * @brief Constructs a ServerInitializationProgressChangedEventArgs.
         * @param name The name of the server
         * @param progress The completion amount of the initialization progress from 0 to 1
         * @param log The log of the initialization progress
         * @param isFinished Whether or not the initialization is completed
         * @param isError Whether or not the initialization completed with an error
         */
        ServerInitializationProgressChangedEventArgs(const std::string& name, double progress, const std::string& log, bool isFinished, bool isError);
        /**
         * @brief Gets the name of the server.
         * @return The name of the server
         */
        const std::string& getName() const;
        /**
         * @brief Gets the completion amount of the initialization progress.
         * @return The completion amount from 0 to 1
         */
        double getProgress() const;
        /**
         * @brief Gets the log of the initialization progres.
         * @return The log of the initialization progress
         */
        const std::string& getLog() const;
        /**
         * @brief Gets whether or not the initialization is completed.
         * @return True if the initialization is completed
         * @return False if the initialization is not yet completed
         */
        bool isFinished() const;
        /**
         * @brief Gets whether or not the initialization completed with an error.
         * @return True if the initialization completed with an error
         * @return False if the initialization completed without an error
         */
        bool isError() const;

    private:
        std::string m_name;
        double m_progress;
        std::string m_log;
        bool m_isFinished;
        bool m_isError;
    };
}

#endif //SERVERINITIALIZATIONPROGRESSCHANGEDEVENTARGS_H
