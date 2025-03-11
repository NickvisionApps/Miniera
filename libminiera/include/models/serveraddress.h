#ifndef SERVERADDRESS_H
#define SERVERADDRESS_H

#include <string>

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of an address of a server.
     */
    class ServerAddress
    {
    public:
        /**
         * @brief Constructs a ServerAddress.
         * @brief The address will be empty.
         */
        ServerAddress();
        /**
         * @brief Constructs a ServerAddress.
         * @param url The url of the address
         * @param port The port of the address
         */
        ServerAddress(const std::string& url, unsigned int port);
        /**
         * @brief Constructs a ServerAddress.
         * @param address The address in the form of (url:port)
         * @throw std::invalid_argument Thrown if the address is ill-formatted.
         */
        ServerAddress(const std::string& address);
        /**
         * @brief Gets whether or not the address is empty.
         * @return True if empty
         * @return False if not empty
         */
        bool isEmpty() const;
        /**
         * @brief Gets the url of the address.
         * @return The address' url
         */
        const std::string& getUrl() const;
        /**
         * @brief Gets the port of the address.
         * @return The address' port
         */
        unsigned int getPort() const;
        /**
         * @brief Gets the address as a string.
         * @return The address as a string
         */
        std::string str() const;
        /**
         * @brief Gets whether or not the address is valid.
         * @return True if valid
         * @return False if not valid
         */
        operator bool() const;

    private:
        std::string m_url;
        unsigned int m_port;
    };
}

#endif //SERVERADDRESS_H
