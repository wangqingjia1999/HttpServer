#pragma once

#include "Request.hpp"
#include "Response.hpp"

#include <memory>

namespace HTTP
{
    class Connection
    {
    public:
        using RequestPtr = std::shared_ptr< Message::Request >;
        using ResponsePtr = std::shared_ptr< Message::Response >;
    public:
        Connection();
        ~Connection();

        Connection(const Connection& other) = default;
        Connection& operator=(const Connection& other) = default;

        Connection(Connection&& other) = default;
        Connection& operator=(Connection&& other) = default;

    public:
        Connection(RequestPtr& request, ResponsePtr& response);

    public:
        void set_request(RequestPtr& request);
        void set_response(ResponsePtr& response);
        RequestPtr& get_request();
        ResponsePtr& get_response();

    public:
        bool operator==(const Connection& other) const;
        bool operator!=(const Connection& other) const;

    private:
        RequestPtr m_request;
        ResponsePtr m_response;
    };
}
