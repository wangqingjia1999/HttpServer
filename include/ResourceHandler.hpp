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

    ResourceHandler(const ResourceHandler& other) = default;
    ResourceHandler& operator=(const ResourceHandler& other) = default;

    ResourceHandler(ResourceHandler&& other) = default;
    ResourceHandler& operator=(ResourceHandler&& other) = default;

public:
    ResourceHandler(ConnectionPtr& connection_ptr);

public:


private:
    ConnectionPtr m_connection_ptr;
};