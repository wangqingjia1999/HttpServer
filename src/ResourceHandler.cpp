#include "ResourceHandler.hpp"

ResourceHandler::ResourceHandler()
{
}

ResourceHandler::~ResourceHandler()
{
}

ResourceHandler::ResourceHandler(const std::string& resource_directory_path)
    : m_resource_root_directory_path(resource_directory_path)
{
}

bool ResourceHandler::is_resource_exists(const std::string& resource_path) const
{
    struct stat resource_buffer;
    return stat(resource_path.c_str(), &resource_buffer) == 0;
}

void ResourceHandler::set_resource_directory_path(const std::string& resource_directory_path)
{
    m_resource_root_directory_path = resource_directory_path;
}

std::string& ResourceHandler::get_resource_directory_path()
{
    return m_resource_root_directory_path;
}

bool ResourceHandler::fetch_resource(std::shared_ptr<Connection>& connection)
{
    std::string resource_absolute_path;

    if(connection->get_request()->get_request_uri()->get_path_string() == "/")
        resource_absolute_path = m_resource_root_directory_path + "index.html";
    else
        resource_absolute_path = m_resource_root_directory_path + formalize_resource_path(connection->get_request()->get_request_uri()->get_path_string());
    
    if(connection->get_request()->has_query())
    {
        // TODO: handle query
        return true;
    }

    if(!is_resource_exists(resource_absolute_path))
    {
        Logger::debug("resource [" + resource_absolute_path + "] doesn't exist.");
        return false;
    }
    
    std::string buffer;
    std::ifstream resource(resource_absolute_path, std::ios_base::binary);
    if(!resource.is_open())
        return false;

    resource.seekg(0, std::ios_base::end);
    auto resource_size = resource.tellg();
    resource.seekg(0);

    buffer.resize(static_cast<size_t>(resource_size));
    
    resource.read(&buffer[0], static_cast<std::streamsize>(resource_size));

    connection->get_response()->set_body(buffer);
    
    connection->get_response()->set_content_type(
        parse_content_type(
            connection->get_request()->get_request_uri()->get_path_string()
        )
    );

    return true;
}

std::string ResourceHandler::formalize_resource_path(const std::string& resource_path)
{
    if(resource_path == "/")
        return std::string{};
    
    if(resource_path[0] == '/')
        return resource_path.substr(1);
    
    return resource_path;
}

std::string ResourceHandler::parse_content_type(const std::string& request_uri_path)
{
    if(request_uri_path == "/")
        return std::string("text/html");

    std::smatch match_result;

    auto regex = std::regex("\\.([0-9a-zA-Z]+)$");

    std::string file_extention;

    if (std::regex_search(request_uri_path, match_result, regex))
    {
        file_extention = match_result[1].str();
    }
    else
    {
        return std::string{};
    }

    if ( file_extention == "txt")
    {
        return "text/plain";
    }
    else if (file_extention == "html" || file_extention == "htm")
    {
        return "text/html";
    }
    else if (file_extention == "css")
    {
        return "text/css";
    }
    else if (file_extention == "jpeg" || file_extention == "jpg")
    {
        return "image/jpg";
    }
    else if (file_extention == "png")
    {
        return "image/png";
    }
    else if (file_extention == "gif")
    {
        return "image/gif";
    }
    else if (file_extention == "svg")
    {
        return "image/svg+xml";
    }
    else if (file_extention == "ico")
    {
        return "image/x-icon";
    }
    else if (file_extention == "json")
    {
        return "application/json";
    }
    else if (file_extention == "pdf")
    {
        return "application/pdf";
    }
    else if (file_extention == "js")
    {
        return "application/javascript";
    }
    else if (file_extention == "wasm")
    {
        return "application/wasm";
    }
    else if (file_extention == "xml")
    {
        return "application/xml";
    }
    else if (file_extention == "xhtml")
    {
        return "application/xhtml+xml";
    }

    return std::string{};
}