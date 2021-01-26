#include "ResourceHandler.hpp"

ResourceHandler::ResourceHandler()
    : m_connection_ptr(nullptr)
{
}

ResourceHandler::~ResourceHandler()
{
    m_connection_ptr->~Connection();
}

ResourceHandler::ResourceHandler(const ResourceHandler& other)
{
    if(this != &other)
        this->m_connection_ptr = other.m_connection_ptr;
}

ResourceHandler& ResourceHandler::operator=(const ResourceHandler& other)
{
    if(this != &other)
        this->m_connection_ptr = other.m_connection_ptr;

    return *this;
}

ResourceHandler::ResourceHandler(ConnectionPtr& connection_ptr)
    : m_connection_ptr(connection_ptr)
{
}

