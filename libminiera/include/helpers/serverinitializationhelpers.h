#ifndef SERVERINITIALIZATIONHELPERS_H
#define SERVERINITIALIZATIONHELPERS_H

#include <filesystem>
#include <string>
#include <libnick/events/event.h>
#include "events/serverinitializationprogresschangedeventargs.h"
#include "models/serverversion.h"

#define SERVER_INITIALIZATION_HELPER_ARGS std::string& log, const std::filesystem::path& serverDir, const std::string& name, const Models::ServerVersion& version, Nickvision::Events::Event<Events::ServerInitializationProgressChangedEventArgs>& progressChanged

namespace Nickvision::Miniera::Shared::Helpers::ServerInitializationHelpers
{
    /**
     * @brief Downloads a server's files.
     * @param log The log variable to write to
     * @param serverDir The server's directory
     * @param name The server's name
     * @param version The server's version
     * @param progressChanged The server's initialization progress changed event
     * @return True if download successful
     * @return False if download failed
     */
    bool download(SERVER_INITIALIZATION_HELPER_ARGS);
    /**
     * @brief Exracts a server's files.
     * @param log The log variable to write to
     * @param serverDir The server's directory
     * @param name The server's name
     * @param version The server's version
     * @param progressChanged The server's initialization progress changed event
     * @return True if extraction successful
     * @return False if extraction failed
     */
    bool extract(SERVER_INITIALIZATION_HELPER_ARGS);
    /**
     * @brief Writes a server's property files.
     * @brief This function writes the eula and server.properties files.
     * @param log The log variable to write to
     * @param serverDir The server's directory
     * @param name The server's name
     * @param version The server's version
     * @param progressChanged The server's initialization progress changed event
     * @return True if writting successful
     * @return False if writting failed
     */
    bool write(SERVER_INITIALIZATION_HELPER_ARGS);
}

#endif //SERVERINITIALIZATIONHELPERS_H
