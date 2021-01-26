#include "Connection.hpp"

Connection::Connection()
	: m_request_ptr(std::make_shared<Message::Request>()),
	  m_response_ptr(std::make_shared<Message::Response>())
{
}

Connection::~Connection()
{
}

Connection::Connection(RequestPtr& request_ptr, ResponsePtr& response_ptr)
    : m_request_ptr(request_ptr),
      m_response_ptr(response_ptr)
{
}

void Connection::set_request(RequestPtr& request_ptr)
{
	m_request_ptr = request_ptr;
}

void Connection::set_response(ResponsePtr& response_ptr)
{
	m_response_ptr = response_ptr;
}

Connection::RequestPtr& Connection::get_request()
{
	return m_request_ptr;
}

Connection::ResponsePtr& Connection::get_response()
{
	return m_response_ptr;
}

bool Connection::operator==(const Connection& other) const
{
	return (this->m_request_ptr == other.m_request_ptr) && (this->m_response_ptr == other.m_response_ptr);
}

bool Connection::operator!=(const Connection& other) const
{
	return (this->m_request_ptr != other.m_request_ptr) && (this->m_response_ptr != other.m_response_ptr);
}