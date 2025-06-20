#include "models/broadcaster.h"
#include <chrono>
#include <stdexcept>
#include <thread>
#include <vector>
#include <boost/json.hpp>
#include <libnick/helpers/stringhelpers.h>
#include <libnick/system/environment.h>

#define NGROK_JSONS_SIZE 6
#define MAX_TRIES 20

using namespace Nickvision::Helpers;
using namespace Nickvision::System;

namespace Nickvision::Miniera::Shared::Models
{
    Broadcaster::Broadcaster(unsigned int port, const std::string& ngrokToken)
        : m_address{ "", 0 }
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

    const ServerAddress& Broadcaster::getAddress() const
    {
        return m_address;
    }

    double Broadcaster::getCPUUsage() const
    {
        return m_proc->getCPUUsage();
    }

    unsigned long long Broadcaster::getRAMUsage() const
    {
        return m_proc->getRAMUsage();
    }

    const ServerAddress& Broadcaster::start()
    {
        //Start ngrok
        m_proc->start();
        //Check for valid initalization
        std::vector<std::string> jsons;
        int tries{ 0 };
        do
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            jsons = StringHelpers::split(m_proc->getOutput(), '\n', false);
            tries++;
        } while(jsons.size() < NGROK_JSONS_SIZE && tries < MAX_TRIES);
        if(jsons.size() < NGROK_JSONS_SIZE)
        {
            m_address = {};
            return m_address;
        }
        //Get WWW url
        boost::json::parser parser;
        try
        {
            parser.write(jsons[jsons.size() - 1]);
        }
        catch(...)
        {
            m_address = {};
            return m_address;
        }
        boost::json::value value = parser.release();
        if(!value.is_object() || !value.as_object()["url"].is_string())
        {
            m_address = {};
            return m_address;
        }
        std::string url{ value.as_object()["url"].as_string() };
        url = url.substr(std::string("tcp://").size());
        m_address = { url };
        return m_address;
    }

    bool Broadcaster::stop()
    {
        if(!m_proc->isRunning())
        {
            return false;
        }
        m_proc->kill();
        m_proc->waitForExit();
        return true;
    }
}
