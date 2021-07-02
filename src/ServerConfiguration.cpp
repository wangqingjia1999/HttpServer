#include "ServerConfiguration.hpp"
#include "Logger.hpp"

#include <error.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

#include <stdexcept>

namespace
{
    void create_folder_if_not_exist(const std::string& folder_path)
    {
        if(mkdir(folder_path.c_str(), S_IRWXU | S_IRWXG ) == -1)
        {
            if(errno != EEXIST)
            {
                throw std::runtime_error(strerror(errno));
            }
        }
        return;
    }
}

namespace
{
    /**
     * Non-standard and hard-coded server root directory path to prevent
     * attacker from gussing it.
     */
    const std::string root_directory_path = { "/home/word-finder/" };

    const std::string resource_directory_path = { root_directory_path + "resource/" };

    const std::string log_directory_path = { root_directory_path + "logs/" };

    const std::string data_storage_directory_path = { "/var/lib/word-finder/" };

    const std::string database_file_path = { data_storage_directory_path + "data.db" };
}

ServerConfiguration::ServerConfiguration()
    : m_root_directory_path{ root_directory_path },
      m_resource_root_directory_path{ resource_directory_path },
      m_database_path{ database_file_path },
      m_log_directory_path{ log_directory_path }
{
    create_folder_if_not_exist(root_directory_path);
    create_folder_if_not_exist(resource_directory_path);
    create_folder_if_not_exist(log_directory_path);
    create_folder_if_not_exist(data_storage_directory_path);
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