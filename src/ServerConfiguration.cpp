#include "ServerConfiguration.hpp"

ServerConfiguration::ServerConfiguration()
    : m_root_directory_path(generate_root_directory_path()),
      m_resource_directory_path(m_root_directory_path + "resource/"),
      m_configuration_file_path(m_root_directory_path + "config")
{
    if(!has_configuration_file())
        create_configuration_file();
    else
        read_configuration_file();
}

ServerConfiguration::~ServerConfiguration()
{
}
 
bool ServerConfiguration::has_configuration_file()
{
    std::ifstream configuration_file_handler(m_configuration_file_path);
    return configuration_file_handler.is_open();
}

std::string ServerConfiguration::generate_root_directory_path()
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

    return current_working_directory_path + '/';
}

void ServerConfiguration::create_configuration_file()
{
    std::ofstream configuration_file_handler(m_configuration_file_path);

    if(configuration_file_handler.is_open())
    {
        configuration_file_handler << "# Type your server's root directory path below\n";
        configuration_file_handler << "root_directory_path: " << m_root_directory_path << '\n';
        configuration_file_handler << "# Type your server's resources directory path below\n";
        configuration_file_handler << "resource_directory_path: " << m_resource_directory_path << '\n';
    }
    else
    {
                // log: cannot create configuration file
        return;
    }
}

std::string ServerConfiguration::get_root_directory_path()
{
    return m_root_directory_path;
}

std::string ServerConfiguration::get_resource_directory_path()
{
    return m_resource_directory_path;
}

void ServerConfiguration::read_configuration_file()
{
    if(!has_configuration_file())
        create_configuration_file();
    
    std::ifstream configuration_file_handler(m_configuration_file_path);
    if(!configuration_file_handler.is_open())
    {
        // log: cannot open configuration file
        return;
    }
    
    std::stringstream configuration_buffer;
    configuration_buffer << configuration_file_handler.rdbuf();
    if(!parse_configuration_file(configuration_buffer.str()))
    {
        // log: cannot parse configuration file
        return;
    }
}

bool ServerConfiguration::parse_configuration_file(const std::string& configuration)
{
    std::string root_directory_path_prefix = "root_directory_path: ";
    if(configuration.find(root_directory_path_prefix) != std::string::npos)
        m_root_directory_path = configuration.substr(
            configuration.find(root_directory_path_prefix),
            root_directory_path_prefix.size()
        );

    std::string resource_directory_path_prefix = "resource_directory_path: ";
    if(configuration.find(resource_directory_path_prefix) != std::string::npos)
        m_resource_directory_path = configuration.substr(
            configuration.find(resource_directory_path_prefix),
            resource_directory_path_prefix.size()
        );

    return true;
}