#include "models/configuration.h"
#include <cmath>
#include <libnick/helpers/sizehelpers.h>
#include <libnick/system/environment.h>
#include <libnick/system/hardwareinfo.h>

using namespace Nickvision::App;
using namespace Nickvision::Helpers;
using namespace Nickvision::System;

namespace Nickvision::Miniera::Shared::Models
{
    Configuration::Configuration(const std::string& key, const std::string& appName)
        : DataFileBase{ key, appName }
    {
        
    }

    Theme Configuration::getTheme() const
    {
        return m_json["Theme"].is_int64() ? static_cast<Theme>(m_json["Theme"].as_int64()) : Theme::System;
    }

    void Configuration::setTheme(Theme theme)
    {
        m_json["Theme"] = static_cast<int>(theme);
    }

    WindowGeometry Configuration::getWindowGeometry() const
    {
        WindowGeometry geometry;
        if(!m_json["WindowGeometry"].is_object())
        {
            geometry.setWidth(800);
            geometry.setHeight(600);
            geometry.setIsMaximized(false);
            return geometry;
        }
        boost::json::object& obj{ m_json["WindowGeometry"].as_object() };
        geometry.setWidth(obj["Width"].is_int64() ? static_cast<long>(obj["Width"].as_int64()) : 800);
        geometry.setHeight(obj["Height"].is_int64() ? static_cast<long>(obj["Height"].as_int64()) : 600);
        geometry.setIsMaximized(obj["IsMaximized"].is_bool() ? obj["IsMaximized"].as_bool() : false);
        return geometry;
    }

    void Configuration::setWindowGeometry(const WindowGeometry& geometry)
    {
        boost::json::object obj;
        obj["Width"] = geometry.getWidth();
        obj["Height"] = geometry.getHeight();
        obj["IsMaximized"] = geometry.isMaximized();
        m_json["WindowGeometry"] = obj;
    }

    bool Configuration::getAutomaticallyCheckForUpdates() const
    {
        return m_json["AutomaticallyCheckForUpdates"].is_bool() ? m_json["AutomaticallyCheckForUpdates"].as_bool() : Environment::getOperatingSystem() == OperatingSystem::Windows;
    }

    void Configuration::setAutomaticallyCheckForUpdates(bool check)
    {
        m_json["AutomaticallyCheckForUpdates"] = check;
    }

    unsigned int Configuration::getMaxServerRamInGB() const
    {
        return m_json["MaxServerRamInGB"].is_uint64() ? static_cast<unsigned int>(m_json["MaxServerRamInGB"].as_uint64()) : static_cast<unsigned int>(std::round(SizeHelpers::bytesToGigabytes(HardwareInfo::getTotalRamSize()) / 4.0));
    }

    void Configuration::setMaxServerRamInGB(unsigned int ram)
    {
        unsigned int systemRam{ static_cast<unsigned int>(SizeHelpers::bytesToGigabytes(HardwareInfo::getTotalRamSize())) };
        if(ram > systemRam)
        {
            ram = static_cast<unsigned int>(std::round(systemRam / 4.0));
        }
        m_json["MaxServerRamInGB"] = ram;
    }

    std::string Configuration::getNgrokAuthToken() const
    {
        return m_json["NgrokAuthToken"].is_string() ? m_json["NgrokAuthToken"].as_string().c_str() : "";
    }

    void Configuration::setNgrokAuthToken(const std::string& token)
    {
        m_json["NgrokAuthToken"] = token;
    }
}
