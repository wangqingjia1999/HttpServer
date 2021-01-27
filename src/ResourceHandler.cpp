#include "ResourceHandler.hpp"

ResourceHandler::ResourceHandler()
{
}

ResourceHandler::~ResourceHandler()
{
}

ResourceHandler::ResourceHandler(const std::string& resource_directory_path)
    : m_resource_directory_path(resource_directory_path)
{
}

bool ResourceHandler::is_resource_exists(const std::string& resource_path) const
{
    struct stat resource_buffer;
    return (stat(resource_path.c_str(), &resource_buffer) == 0);
}

void ResourceHandler::set_resource_directory_path(const std::string& resource_directory_path)
{
    m_resource_directory_path = resource_directory_path;
}

std::string& ResourceHandler::get_resource_directory_path()
{
    return m_resource_directory_path;
}

bool ResourceHandler::fetch_resource(std::shared_ptr<Connection>& connection)
{
    if(!is_resource_exists(m_resource_directory_path + connection->get_request()->get_request_uri()))
        return false;
    
    
}