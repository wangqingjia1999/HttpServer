#pragma once

#include <string>

class ServerConfiguration
{
public:
    ~ServerConfiguration() = default;

protected:
    ServerConfiguration();
    
    ServerConfiguration(const ServerConfiguration& other) = default;
    ServerConfiguration& operator=(const ServerConfiguration& other) = default;
    
    ServerConfiguration(ServerConfiguration&& other) = default;
    ServerConfiguration& operator=(ServerConfiguration&& other) = default;

public:
    std::string get_database_path() const;
    std::string get_root_directory_path() const;
    std::string get_resource_directory_path() const;
    std::string get_log_directory_path() const;
    static ServerConfiguration* instance();

private:
    std::string m_root_directory_path;
    std::string m_resource_root_directory_path;
    std::string m_log_directory_path;
    std::string m_database_path;
    static ServerConfiguration* m_instance;
};