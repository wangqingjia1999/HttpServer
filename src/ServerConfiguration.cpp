#include "ServerConfiguration.hpp"

ServerConfiguration::ServerConfiguration()
    : m_root_directory_path(get_root_directory_path()),
      m_resource_directory_path(get_root_directory_path() + "resource/")
{
    if(!has_configuration_file())
        create_configuration_file();
    
}

ServerConfiguration::~ServerConfiguration()
{
}
 
bool ServerConfiguration::has_configuration_file()
{
    std::ifstream configuration_file_handler(get_root_directory_path() + "config");
    return configuration_file_handler.is_open();
}

std::string ServerConfiguration::get_root_directory_path()
{
    char path_buffer[1024];
    if(getcwd(path_buffer, 1024) == nullptr)
    {
        // log: can not create server configuration file
    }

    std::string current_working_directory_path(path_buffer);
    
    if(current_working_directory_path.find("HttpServer") == std::string::npos)
    {
        // log: get working directory error
    }

    current_working_directory_path = current_working_directory_path.substr(0, current_working_directory_path.find("HttpServer") + 10);

    printf("Server root directory path is: %s\n", current_working_directory_path.c_str());

    return current_working_directory_path + '/';
}

void ServerConfiguration::create_configuration_file()
{
    std::ofstream configuration_file_handler(get_root_directory_path() + "config");

    if(!configuration_file_handler.is_open())
    {
        printf("Can not create configuration file.\n");
        return;
    }
    else
    {
        printf("Open configuration file successfully.\n");

        configuration_file_handler << "# Type your server's root directory path below\n";
        configuration_file_handler << m_root_directory_path << '\n';
        configuration_file_handler << "# Type your server's resources directory path below\n";
        configuration_file_handler << m_resource_directory_path << '\n';
    }
}