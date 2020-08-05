#ifndef WEBSOCKET_HPP
#define WEBSOCKET_HPP

#include <functional>

#include <string>
#include <memory>

#include "Response.hpp"
#include "Request.hpp"

/**
 * TODO:
 *  1. Fragmentation
 *  2. Send/Receive to/from client.
 */
class websocket
{
public:
    ~websocket() noexcept;
    websocket();
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
     * @param[in] file_descriptor
     *      The file descriptor of receiver.
     * @param[in] data
     *      This is the data to be included in the message.
     * @param[in] is_last_fragment
     *      This indicates whether or not it's 
     *      the last frame in the message.
     */
    void send_binary(
        const int file_descriptor,
        const std::string& data,
        bool is_last_fragment = false
    );

    /**
     * Send a text message over the WebSocket.
     * 
     * @param[in] file_descriptor
     *      The file descriptor of receiver.
     * @param[in] data
     *      This is the data to be included in the message.
     * @param[in] is_last_fragment
     *      Whether or not this is the last fragment in the message.
     */
    void send_text(
        const int file_descriptor,
        const std::string& data,
        bool is_last_fragment = false
    );

    bool parse_websocket_request();
    bool generate_websocket_request();
    /**
     * Close WebSocket connection.
     */
    void close_connection(
        unsigned int code = 1005,
        const std::string& reason = ""
    );

    /**
     * Generate WebSocket Key for Sec-WebSocket-Accept header's value,
     * based on the request's Sec-WebSocket-Key field.
     * 
     * @return std::string
     *      WebSocket Key to be filled in Sec-WebSocket-Accept of response.
     */
    std::string generate_sec_websocket_key();

    /**
     * Generate WebSocket Key for Sec-WebSocket-Accept header's value,
     * based on given string.
     * 
     * @param[in] key_string
     *      Given key string.
     * @return std::string
     *      WebSocket Key to be filled in Sec-WebSocket-Accept of response.
     */
    std::string generate_sec_websocket_key(const std::string& key_string);

private:
    struct Impl;
    std::unique_ptr< Impl > impl_;
};

#endif