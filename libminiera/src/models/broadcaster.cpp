#include "models/broadcaster.h"
#include <chrono>
#include <stdexcept>
#include <thread>
#include <vector>
#include <boost/json.hpp>
#include <libnick/helpers/stringhelpers.h>
#include <libnick/system/environment.h>

#define NGROK_JSONS_SIZE 6

using namespace Nickvision::Helpers;
using namespace Nickvision::System;

namespace Nickvision::Miniera::Shared::Models
{
    Broadcaster::Broadcaster(unsigned int port, const std::string& ngrokToken)
    {
        if(ngrokToken.empty())
        {
            throw std::invalid_argument("The ngrok token is invalid");
        }
        m_proc = std::make_shared<Process>(Environment::findDependency("ngrok"), std::vector<std::string>{ "tcp", std::to_string(port), "--authtoken", ngrokToken, "--log", "stdout", "--log-format", "json" });
    }

    Broadcaster::~Broadcaster()
    {
        stop();
    }

    const std::string& Broadcaster::getUrl() const
    {
        return m_url;
    }

    const std::string& Broadcaster::start()
    {
        //Start ngrok
        m_proc->start();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //Check for valid initalization
        std::vector<std::string> jsons{ StringHelpers::split(m_proc->getOutput(), "\n") };
        if(jsons.size() != NGROK_JSONS_SIZE)
        {
            m_url = "";
            return m_url;
        };
        //Get WWW url
        boost::json::parser parser;
        parser.write(jsons[NGROK_JSONS_SIZE - 1]);
        boost::json::value value = parser.release();
        if(!value.is_object() || !value.as_object()["url"].is_string())
        {
            m_url = "";
            return m_url;
        }
        std::string url{ value.as_object()["url"].as_string() };
        m_url = url.substr(std::string("tcp://").size());
        return m_url;
    }

    bool Broadcaster::stop()
    {
        if(!m_proc || !m_proc->isRunning())
        {
            return false;
        }
        m_proc->kill();
        m_proc->waitForExit();
        return true;
    }
}
