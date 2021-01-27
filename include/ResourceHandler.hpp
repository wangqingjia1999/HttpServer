#pragma once

#include "Connection.hpp"

#include <string>
#include <memory>
#include <sys/stat.h>

/**
 * This class is responsible for managing server side resources, mainly for I/O.
 */
class ResourceHandler
{
public:
    ResourceHandler();
    ~ResourceHandler();

    ResourceHandler(const ResourceHandler& other) = default;
    ResourceHandler& operator=(const ResourceHandler& other) = default;

    ResourceHandler(ResourceHandler&& other) = default;
    ResourceHandler& operator=(ResourceHandler&& other) = default;

public:
    ResourceHandler(const std::string& resource_directory_path);

public:
    /**
     * Fetch resource based on given connection's request
     * and fill the resource into the connection's response.
     * 
     * @return 
     *      True if succeeds.
     */
    bool fetch_resource(std::shared_ptr<Connection>& connection);

    /**
     * Whether the resource exists on the server.
     *
     * @param[in] resource_path
     *      Path that is relative to the m_resource_root_directory_path.
     * 
     * @return
     *      True if the resource exists.
     */
    bool is_resource_exists(const std::string& resource_path) const;

    /**
     * Formalize given resource path.
     * 
     * @param[in] resource_path
     *      Resource path.
     *
     * @return
     *      Path that is relative to m_resource_root_directory_path.
     */
    std::string formalize_resource_path(const std::string& resource_path);

    /**
     * Determine content type based on the file extension.
     * 
     * @param[in] request_uri  
     * 		Request URI.
     * 
     * @return
     *      Content type.
     * 
     * @note 
     * 		For all available content types, see:
     * 		https://www.iana.org/assignments/media-types/media-types.xhtml#application
     */
    std::string parse_content_type(const std::string& request_uri);

    void set_resource_directory_path(const std::string& resource_directory_path);
    std::string& get_resource_directory_path();
    
private:
    std::string m_resource_root_directory_path;
};