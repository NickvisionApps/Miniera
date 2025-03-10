#ifndef SERVER_H
#define SERVER_H

#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <boost/json.hpp>
#include <libnick/events/event.h>
#include <libnick/system/process.h>
#include "events/serverinitializationprogresschangedeventargs.h"
#include "broadcaster.h"
#include "serverproperties.h"
#include "serverversion.h"

namespace Nickvision::Miniera::Shared::Models
{
    /**
     * @brief A model of a minecraft server.
     */
    class Server
    {
    public:
        /**
         * @brief Constructs a Server.
         * @param version The version of the server
         * @param properties The properties of the server
         * @param directory The directory of the server
         */
        Server(const ServerVersion& version, const ServerProperties& properties, const std::filesystem::path& directory);
        /**
         * @brief Constructs a Server.
         * @param json The json object to construct the server from
         * @throw std::invalid_argument Thrown if the json object is invalid
         */
        Server(boost::json::object json);
        /**
         * @brief Destructs a Server
         */
        ~Server();
        /**
         * @brief Gets whether or not there is a server running.
         * @return True if there is a server running
         * @return False if there is not a server running
         */
        static bool isServerRunning();
        /**
         * @brief Gets the event for when the initialization's progress is changed.
         * @return The initialization progress changed event.
         */
        Nickvision::Events::Event<Events::ServerInitializationProgressChangedEventArgs>& initializationProgressChanged();
        /**
         * @brief Gets the name of the server.
         * @return The name of the server
         */
        const std::string& getName() const;
        /**
         * @brief Gets the version of the server.
         * @return The version of the server
         */
        const ServerVersion& getVersion() const;
        /**
         * @brief Gets the console output of the server.
         * @return The console output of the server
         */
        const std::string& getOutput() const;
        /**
         * @brief Gets the url of the server.
         * @return The url of the server
         */
        std::pair<std::string, unsigned int> getUrl() const;
        /**
         * @brief Gets whether or not the server is running.
         * @return True if the server is running
         * @return False if the server is not running
         */
        bool isRunning() const;
        /**
         * @brief Initializes the server.
         * @brief This will download and setup the main server executable and directories.
         * @brief This function runs on another thread. Progress should be tracked via the ServerInitializationProgressChanged event.
         */
        void initialize();
        /**
         * @brief Starts the server.
         * @brief This function will return false if another server is running as only one server may be running at a time.
         * @param maxServerRamInGB The maximum amount of ram to use for the server (in GB)
         * @return True if the server was started or is already running
         * @return False if the server was not started
         */
        bool start(unsigned int maxServerRamInGB);
        /**
         * @brief Stops the server.
         * @return True if the server was stopped or is not running
         * @return False if the server was not stopped
         */
        bool stop();
        /**
         * @brief Sends a command to the server.
         * @param cmd The command to send
         * @return True if the command was sent (this does not mean the command was valid)
         * @return False if the command was not sent
         */
        bool command(const std::string& cmd);
        /**
         * @brief Broadcasts the server over the WWW.
         * @param ngrokToken The user's api token for the ngrok broadcaster
         * @return The WWW url of the server
         * @return Empty string on error
         */
        const std::string& broadcast(const std::string& ngrokToken);
        /**
         * @brief Converts the Server to a JSON object.
         * @return The Server as a JSON object
         */
        boost::json::object toJson() const;
        /**
         * @brief Compares this Server to other Server via ==.
         * @param other The other Server
         * @return True if this == other
         */
        bool operator==(const Server& other) const;
        /**
         * @brief Compares this Server to other Server via !=.
         * @param other The other Server
         * @return True if this != other
         */
        bool operator!=(const Server& other) const;

    private:
        /**
         * @brief Writes customizable server files to disk.
         * @return True if successful
         * @return False if unsuccessful
         */
        bool wrtieFilesToDisk();
        /**
         * @brief Checks if a server has been initalized or not.
         * @return True if the server has been initialized
         * @return False if the server has not been initialized
         */
        bool initializeCheck();
        /**
         * @brief Downloads a server's files.
         * @param log The log variable to write to
         * @return True if download successful
         * @return False if download failed
         */
        bool initializeDownload(std::string& log);
        /**
         * @brief Exracts a server's files.
         * @param log The log variable to write to
         * @return True if extraction successful
         * @return False if extraction failed
         */
        bool initializeExtract(std::string& log);
        /**
         * @brief Writes a server's property files.
         * @brief This function writes the eula and server.properties files.
         * @param log The log variable to write to
         * @return True if writting successful
         * @return False if writting failed
         */
        bool initializeWrite(std::string& log);
        mutable std::mutex m_mutex;
        ServerVersion m_version;
        ServerProperties m_properties;
        std::filesystem::path m_directory;
        bool m_initialized;
        std::shared_ptr<System::Process> m_proc;
        std::shared_ptr<Broadcaster> m_broadcaster;
        Nickvision::Events::Event<Events::ServerInitializationProgressChangedEventArgs> m_initializationProgressChanged;
    };
}

#endif //SERVER_H
