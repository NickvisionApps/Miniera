#include "controllers/preferencesviewcontroller.h"
#include <libnick/helpers/sizehelpers.h>
#include <libnick/system/hardwareinfo.h>

using namespace Nickvision::Helpers;
using namespace Nickvision::Miniera::Shared::Models;
using namespace Nickvision::System;

namespace Nickvision::Miniera::Shared::Controllers
{
    PreferencesViewController::PreferencesViewController(Configuration& configuration)
        : m_configuration{ configuration }
    {

    }
    
    Theme PreferencesViewController::getTheme() const
    {
        return m_configuration.getTheme();
    }

    void PreferencesViewController::setTheme(Theme theme)
    {
        m_configuration.setTheme(theme);
    }

    bool PreferencesViewController::getAutomaticallyCheckForUpdates() const
    {
        return m_configuration.getAutomaticallyCheckForUpdates();
    }

    void PreferencesViewController::setAutomaticallyCheckForUpdates(bool check)
    {
        m_configuration.setAutomaticallyCheckForUpdates(check);
    }

    std::string PreferencesViewController::getNgrokAuthToken() const
    {
        return m_configuration.getNgrokAuthToken();
    }

    unsigned int PreferencesViewController::getMaxServerRamInGB() const
    {
        return m_configuration.getMaxServerRamInGB();
    }

    void PreferencesViewController::setMaxServerRamInGB(unsigned int ram)
    {
        m_configuration.setMaxServerRamInGB(ram);
    }

    unsigned int PreferencesViewController::getSystemRamInGB() const
    {
        return static_cast<unsigned int>(SizeHelpers::bytesToGigabytes(HardwareInfo::getTotalRamSize()));
    }

    void PreferencesViewController::setNgrokAuthToken(const std::string& token)
    {
        m_configuration.setNgrokAuthToken(token);
    }

    const std::string& PreferencesViewController::getNgrokAuthTokenRetrievalUrl() const
    {
        static std::string url{ "https://dashboard.ngrok.com/get-started/your-authtoken" };
        return url;
    }

    void PreferencesViewController::saveConfiguration()
    {
        m_configuration.save();
    }
}
