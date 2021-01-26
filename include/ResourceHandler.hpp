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
     * @return
     *      True if the resource exists.
     */
    bool is_resource_exists(const std::string& resource_path) const;

    void set_resource_directory_path(const std::string& resource_directory_path);
    std::string& get_resource_directory_path();
    
private:
    std::string m_resource_directory_path;
};