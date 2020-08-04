#ifndef WEBSOCKET_HPP
#define WEBSOCKET_HPP

#include <functional>

#include <string>
#include <memory>

#include "Response.hpp"
#include "Request.hpp"


class websocket
{
public:
    ~websocket() noexcept;
    websocket() = delete;
    websocket(
        std::shared_ptr< Message::Request > request,
        std::shared_ptr< Message::Response > response
    );

    websocket(const websocket&) = delete;
    websocket& operator=(const websocket&) = delete;

    websocket(websocket&&) noexcept;
    websocket& operator=(websocket&&) noexcept;
public:
    /**
     * Send a ping message over the WebSocket.
     * 
     * @param[in] data
     *      This is the optional data to be included in the message. 
     */
    void ping(const std::string& data = "");

    /**
     * Send a poing unsolicited pong message over WebSocket.
     * 
     * @param[in] data
     *      This is the optional data to be included in the message.
     */
    void pong(const std::string& data = "");

    /**
     * Send a binary message over the WebSocket.
     * 
     * @param[in] data
     *      This is the data to be included in the message.
     * 
     * @param[in] is_last_fragment
     *      This indicates whether or not it's 
     *      the last frame in the message.
     */
    void send_binary(
        const std::string& data,
        bool is_last_fragment = true
    );

    /**
     * Send a text message over the WebSocket.
     * 
     * @param[in] data
     *      This is the data to be included in the message.
     * 
     * @param[in] is_last_fragment
     *      Whether or not this is the last fragment in the message.
     */
    void send_text();

    bool generate_websocket_request();

    bool parse_websocket_request();
    
    /**
     * Close WebSocket connection.
     */
    void close_connection(
        unsigned int code = 1005,
        const std::string& reason = ""
    );

    std::string generate_sec_websocket_key();

private:
    struct Impl;
    std::unique_ptr< Impl > impl_;
};

#endif