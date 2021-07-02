#include "ServerConfiguration.hpp"
#include "Logger.hpp"

#include <error.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

#include <stdexcept>
#include <fstream>

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

    void create_file_if_not_exist(const std::string& file_path, const std::string& file_content)
    {
        std::ofstream output_file(file_path);
        if(!output_file.is_open())
        {
            throw std::runtime_error("can't create file: " + file_path);
        }

        output_file << file_content;
        output_file.close();
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

    const std::string asset_directory_path = { resource_directory_path + "assets/" };

    const std::string css_directory_path = { asset_directory_path + "css/" };

    const std::string html_directory_path = { asset_directory_path + "html/" };

    const std::string js_directory_path = { asset_directory_path + "js/" };

    const std::string image_directory_path = { asset_directory_path + "image/" };

    const std::string log_directory_path = { root_directory_path + "logs/" };

    const std::string data_storage_directory_path = { "/var/lib/word-finder/" };

    const std::string database_file_path = { data_storage_directory_path + "data.db" };
}

namespace
{
    const std::string default_home_html {
        "<!DOCTYPE html>"
        "<html lang=\"en\">"
            "<head>"
                "<meta charset=\"utf-8\">"
                "<title>Home</title>"
                "<link rel=\"stylesheet\" href=\"./assets/css/HomePage.css\">"
            "</head>"

            "<body>"
                "<div id=\"sign-up\">"
                    "<a href=\"assets/html/SignUp.html\">Sign Up</a>"
                "</div>"

                "<span id=\"site-logo-banner\">"
                    "<h1>Word/Phrase Finder</h1>"
                    "<p>Find real-life English sentences taken from reliable sources to facilitate English learning.</p>"
                "</span>"
                
                "<div id=\"search-bar-wrap\">"
                    "<form role=\"search\" method=\"GET\" action=\".\">"
                        "<input type=\"search\" id=\"search-bar-input\" placeholder=\"Type a word or phrase here\" name=\"q\">"
                        "<button type=\"submit\" id=\"search-bar-submit-button\">Go</button>"
                    "</form>"
                "</div>"
            "</body>"

        "</html>"
    };

    const std::string default_home_css {
        "body {"
            "height: 100%;"
            "margin: 0px;"
            "background-repeat: no-repeat;"
            "background-size: cover;"
        "}"

        "#search-bar-wrap {"
            "width: 30%;"
            "position: absolute;"
            "top: 40%;"
            "left: 50%;"
            "transform: translate(-50%, -50%);"
        "}"

        "#search-bar-input {"
            "width: 100%;"
            "height: 44px;"
            "border: 1px solid #000000;"
            "border-right: none;"
            "padding: 5px;"
            "border-radius: 5px 0 0 5px;"
            "outline: none;"
            "color: #000000;"
        "}"


        "#search-bar-submit-button {"
            "width: 40px;"
            "height: 44px;"
            "border: 1px solid #000000;"
            "background: center;"
            "text-align: center;"
            "color: rgb(0, 0, 0);"
            "border-radius: 0 5px 5px 0;"
            "cursor: pointer;"
            "font-size: 20px;"
            "position: absolute;"
        "}"

        "#site-logo-banner {"
            "text-align: center;"
        "}"

        "#sign-up{"
            "text-align: right;"
            "margin-right: 5px;"
            "margin-top: 5px;"
        "}"
    };
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
    create_folder_if_not_exist(asset_directory_path);
    create_folder_if_not_exist(css_directory_path);
    create_folder_if_not_exist(html_directory_path);
    create_folder_if_not_exist(js_directory_path);
    create_folder_if_not_exist(image_directory_path);
    create_file_if_not_exist(resource_directory_path + "index.html", default_home_html);
    create_file_if_not_exist(css_directory_path + "HomePage.css", default_home_css);
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