#include "ServerConfiguration.hpp"

namespace
{
    std::string strip_leading_and_pending_spaces(std::string input_string)
    {
        std::size_t leading_space_index = input_string.find_first_not_of(' ');
        std::size_t pending_space_index = input_string.find_last_not_of(' ');
        return input_string.substr(leading_space_index, pending_space_index);
    }
}

ServerConfiguration::ServerConfiguration()
    : m_root_directory_path(generate_root_directory_path()),
      m_configuration_file_path(m_root_directory_path + "config"),
      m_resource_root_directory_path(m_root_directory_path + "resource/")
{
    if(!has_configuration_file())
        create_configuration_file();
    else
        parse_configuration_file();
}

ServerConfiguration::~ServerConfiguration()
{
}
 
bool ServerConfiguration::has_configuration_file() const
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
        configuration_file_handler << "# server's root directory path\n";
        configuration_file_handler << "root_directory_path: " << m_root_directory_path << '\n';
        configuration_file_handler << "# server's resources directory path\n";
        configuration_file_handler << "resource_directory_path: " << m_resource_root_directory_path << '\n';
        configuration_file_handler << "# server's log files directory path\n";
        configuration_file_handler << "log_directory_path: " << m_log_directory_path << '\n';
    }
    else
    {
        // cannot create configuration file
        return;
    }
}

std::string ServerConfiguration::get_root_directory_path() const
{
    return m_root_directory_path;
}

std::string ServerConfiguration::get_resource_directory_path() const
{
    return m_resource_root_directory_path;
}

std::string ServerConfiguration::get_log_directory_path() const
{
    return m_log_directory_path;
}

void ServerConfiguration::parse_configuration_file()
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
    parse_configuration(configuration_buffer.str());
}

void ServerConfiguration::parse_configuration(const std::string& configuration)
{
    std::smatch matcher;
    std::regex_search(
        configuration, 
        matcher, 
        std::regex("root_directory_path:(.*?)\n")
    );    
    
    if(!matcher.empty())
        m_root_directory_path = strip_leading_and_pending_spaces(matcher[1]);

    std::regex_search(
        configuration,
        matcher,
        std::regex("resource_directory_path:(.*?)\n")
    );
    
    if(!matcher.empty())
        m_resource_root_directory_path = strip_leading_and_pending_spaces(matcher[1]);

    std::regex_search(
        configuration,
        matcher,
        std::regex("log_directory_path:(.*?)\n")
    );

    if(!matcher.empty())
        m_log_directory_path = strip_leading_and_pending_spaces(matcher[1]);
}