#include "models/serveraddress.h"
#include <sstream>
#include <stdexcept>
#include <libnick/helpers/stringhelpers.h>

using namespace Nickvision::Helpers;

namespace Nickvision::Miniera::Shared::Models
{
    ServerAddress::ServerAddress()
        : m_url{ "" },
        m_port{ 0 }
    {

    }

    ServerAddress::ServerAddress(const std::string& url, unsigned int port)
        : m_url{ url },
        m_port{ port }
    {

    }

    ServerAddress::ServerAddress(const std::string& address)
    {
        std::vector<std::string> fields{ StringHelpers::split(address, ':', false) };
        if(fields.size() != 2)
        {
            throw std::invalid_argument("Invalid address format");
        }
        m_url = fields[0];
        m_port = static_cast<unsigned int>(std::stoul(fields[1]));
    }

    bool ServerAddress::isEmpty() const
    {
        return m_url.empty() || m_port == 0;
    }

    const std::string& ServerAddress::getUrl() const
    {
        return m_url;
    }

    unsigned int ServerAddress::getPort() const
    {
        return m_port;
    }

    std::string ServerAddress::str() const
    {
        std::stringstream builder;
        builder << m_url << ":" << m_port;
        return builder.str();
    }

    bool ServerAddress::operator==(const ServerAddress& other) const
    {
        return m_url == other.m_url && m_port == other.m_port;
    }

    bool ServerAddress::operator!=(const ServerAddress& other) const
    {
        return !(operator==(other));
    }

    ServerAddress::operator bool() const
    {
        return !isEmpty();
    }
}
