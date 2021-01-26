#pragma once

#include "Request.hpp"
#include "Response.hpp"

#include <memory>

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
    Connection(RequestPtr& request_ptr, ResponsePtr& response_ptr);

public:
    void set_request(RequestPtr& request_ptr);
    void set_response(ResponsePtr& response_ptr);
    RequestPtr& get_request();
    ResponsePtr& get_response();

    // Operator overloading
public:
    bool operator==(const Connection& other) const;
    bool operator!=(const Connection& other) const;

private:
    RequestPtr m_request_ptr;
    ResponsePtr m_response_ptr;
};