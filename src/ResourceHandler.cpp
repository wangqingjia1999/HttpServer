#include "ResourceHandler.hpp"

ResourceHandler::ResourceHandler()
    : m_connection_ptr(nullptr)
{
}

ResourceHandler::~ResourceHandler()
{
    m_connection_ptr->~Connection();
}

ResourceHandler::ResourceHandler(ConnectionPtr& connection_ptr)
    : m_connection_ptr(connection_ptr)
{
}

