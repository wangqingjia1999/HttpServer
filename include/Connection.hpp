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

    // Operator overloading
public:

private:
    RequestPtr m_request_ptr;
    ResponsePtr m_response_ptr;
};