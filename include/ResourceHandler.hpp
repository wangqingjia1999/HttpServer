#pragma once

#include "Connection.hpp"

#include <memory>

class ResourceHandler
{
public:
    using ConnectionPtr = std::shared_ptr< Connection >;

public:
    ResourceHandler();
    ~ResourceHandler();

    ResourceHandler(const ResourceHandler& other);
    ResourceHandler& operator=(const ResourceHandler& other);

    ResourceHandler(ResourceHandler&& other) = delete;
    ResourceHandler& operator=(ResourceHandler&& other) = delete;

public:
    ResourceHandler(ConnectionPtr& connection_ptr);

public:


private:
    ConnectionPtr m_connection_ptr;
};