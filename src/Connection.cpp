#include "Connection.hpp"

Connection::Connection(RequestPtr& request_ptr, ResponsePtr& response_ptr)
    : m_request_ptr(request_ptr),
      m_response_ptr(response_ptr)
{
}