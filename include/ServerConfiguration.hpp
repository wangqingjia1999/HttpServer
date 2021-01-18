#pragma once

#include <string>
#include <fstream>

#include <unistd.h>

class ServerConfiguration
{
public:
    ServerConfiguration();
    ~ServerConfiguration();

public:
    std::string get_root_directory_path();
    std::string get_resource_directory_path();
    bool has_configuration_file();
    void create_configuration_file();
    void read_configuration_file();
    
private:
    std::string m_root_directory_path;
    std::string m_resource_directory_path;
};