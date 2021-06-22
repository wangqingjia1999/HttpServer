#pragma once

#include <regex>
#include <string>
#include <fstream>
#include <sstream>

#include <unistd.h>

class ServerConfiguration
{
public:
    ServerConfiguration();
    ~ServerConfiguration();

public:
    std::string get_database_path() const;
    std::string get_root_directory_path() const;
    std::string get_resource_directory_path() const;
    std::string get_log_directory_path() const;

    /**
     * Get server's root directory path. e.g. /home/usernam/HttpServer
     * 
     * @return
     *      Path string if any.
     */
    std::string generate_root_directory_path();
    
    /**
     * Whether the server has a configuration file.
     * 
     * @return
     *      True if it has.
     */
    bool has_configuration_file() const;

    /**
     * Create a configuration file under root directory.
     * 
     * @note
     *      Please first fill member variables then call this.
     */
    void create_configuration_file();

    /**
     * Parse configuration file and fill into member variables.
     */
    void parse_configuration_file();

    /**
     * Parse given configuration string.
     * 
     * @param[in] configuration
     *      Configuration string.
     */
    void parse_configuration(const std::string& configuration);

private:
    std::string m_root_directory_path;
    std::string m_configuration_file_path;
    std::string m_resource_root_directory_path;
    std::string m_log_directory_path;
    std::string m_database_path;
    // TODO: Uri maximum length;
};