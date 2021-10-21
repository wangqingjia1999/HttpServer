#pragma once

#include "Request.hpp"
#include "Response.hpp"

#include <memory>

namespace HTTP
{
	class Connection
	{
	public:
		using RequestPtr = std::shared_ptr<Message::Request>;
		using ResponsePtr = std::shared_ptr<Message::Response>;

		Connection();
		~Connection() = default;

		Connection(const Connection& other) = default;
		Connection& operator=(const Connection& other) = default;

		Connection(Connection&& other) = default;
		Connection& operator=(Connection&& other) = default;

		Connection(RequestPtr request, ResponsePtr response);

		void set_request(const RequestPtr& request);
		void set_response(const ResponsePtr& response);
		RequestPtr& get_request();
		ResponsePtr& get_response();

		bool operator==(const Connection& other) const;
		bool operator!=(const Connection& other) const;

	private:
		RequestPtr m_request;
		ResponsePtr m_response;
	};
} // namespace HTTP
