#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <memory>
#include <string>
#include <libnick/system/process.h>

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of an ngrok broascaster.
     */
    class Broadcaster
    {
    public:
        /**
         * @brief Constructs a Broadcaster.
         * @param port The port to broadcast
         * @param ngrokToken The user's ngrok api token
         * @throw std::invalid_argument Thrown if the ngrok token is invalid
         */
        Broadcaster(unsigned int port, const std::string& ngrokToken);
        /**
         * @brief Destructs a Broadcaster.
         */
        ~Broadcaster();
        /**
         * @brief Gets the WWW url of the broadcasted port.
         * @return The WWW url
         */
        const std::string& getUrl() const;
        /**
         * @brief Starts the broadcaster.
         * @return The WWW url of the broadcasted port
         * @return Empty string on erorr
         */
        const std::string& start();
        /**
         * @brief Stops the broadcaster.
         * @return True if successful
         * @return False if unsuccessful
         */
        bool stop();

    private:
        std::shared_ptr<System::Process> m_proc;
        std::string m_url;
    };
}

#endif //BROADCASTER_H
