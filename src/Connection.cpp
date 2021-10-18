#include "Connection.hpp"

namespace HTTP
{
	Connection::Connection()
	    : m_request(std::make_shared<Message::Request>())
	    , m_response(std::make_shared<Message::Response>())
	{
	}

	Connection::~Connection() {}

	Connection::Connection(RequestPtr& request, ResponsePtr& response)
	    : m_request(request)
	    , m_response(response)
	{
	}

	void Connection::set_request(RequestPtr& request) { m_request = request; }

	void Connection::set_response(ResponsePtr& response)
	{
		m_response = response;
	}

	Connection::RequestPtr& Connection::get_request() { return m_request; }

	Connection::ResponsePtr& Connection::get_response() { return m_response; }

	bool Connection::operator==(const Connection& other) const
	{
		return (this->m_request == other.m_request) &&
		       (this->m_response == other.m_response);
	}

	bool Connection::operator!=(const Connection& other) const
	{
		return (this->m_request != other.m_request) &&
		       (this->m_response != other.m_response);
	}
} // namespace HTTP
