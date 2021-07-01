#include "ServerConfiguration.hpp"

namespace
{
    /**
     * Non-standard and hard-coded server root directory path to prevent
     * attacker from gussing it.
     */
    const std::string root_directory_path = { "/home/word-finder/" };

    const std::string configuration_file_path = { root_directory_path + "config" };

    const std::string resource_directory_path = { root_directory_path + "resource" };

    const std::string log_directory_path = { root_directory_path + "logs" };

    const std::string database_file_path = { root_directory_path + "data.db" };
}

ServerConfiguration::ServerConfiguration()
    : m_root_directory_path{ root_directory_path },
      m_configuration_file_path{ configuration_file_path },
      m_resource_root_directory_path{ resource_directory_path },
      m_database_path{ database_file_path },
      m_log_directory_path{ log_directory_path }
{
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

std::string ServerConfiguration::get_database_path() const
{
    return m_database_path;
}