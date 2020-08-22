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
class WebSocket
{
public:
    ~WebSocket() noexcept;
    WebSocket();
    WebSocket(
        std::shared_ptr< Message::Request > request,
        std::shared_ptr< Message::Response > response
    );

    WebSocket(const WebSocket&) = delete;
    WebSocket& operator=(const WebSocket&) = delete;

    WebSocket(WebSocket&&) noexcept;
    WebSocket& operator=(WebSocket&&) noexcept;
public:
    /**
     * @brief  Send a ping message over the WebSocket.
     * @param  data  Optional data to be included in the message. 
     */
    void ping(const std::string& data = "");

    /**
     * Send a poing unsolicited pong message over WebSocket.
     * 
     * @param  data  Optional data to be included in the message.
     */
    void pong(const std::string& data = "");

    /**
     * @brief  Send a binary message over the WebSocket.
     * @param  file_descriptor  The file descriptor of receiver.
     * @param  data  Data to be included in the message.
     * @param  is_last_fragment  Whether or not it's the last frame in the message.
     */
    void send_binary(
        const int file_descriptor,
        const std::string& data,
        bool is_last_fragment = false
    );

    /**
     * @brief  Send a text message over the WebSocket.
     * @param  file_descriptor  The file descriptor of receiver.
     * @param  data  Data to be included in the message.
     * @param  is_last_fragment  Whether or not this is the last fragment in the message.
     */
    void send_text(
        const int file_descriptor,
        const std::string& data,
        bool is_last_fragment = false
    );

    /**
     * @brief  Parse WebSocket request.
     * @return  True if successfully parse WebSocket request.
     */
    bool parse_websocket_request();

    /**
     * @brief  Generate WebSocket request.
     * @return  True if successfully generate WebSocket request.
     */
    bool generate_websocket_request();
    
    /**
     * @brief  Close WebSocket connection.
     * @param  code  Close flag code, with default value of 1005.
     * @param  reason  Reason phrase.
     */
    void close_connection(
        unsigned int code = 1005,
        const std::string& reason = ""
    );

    /**
     * @brief  Generate WebSocket Key
     * @return  WebSocket Key.
     * 
     * Generate WebSocket Key for Sec-WebSocket-Accept header's value,
     * based on the request's Sec-WebSocket-Key field.
     */
    std::string generate_sec_websocket_key();

    /**
     * @brief  Generate WebSocket Key
     * @param  key_string  Given key string.
     * @return  WebSocket Key to be filled in Sec-WebSocket-Accept of response.
     */
    std::string generate_sec_websocket_key(const std::string& key_string);

private:
    struct Impl;
    std::unique_ptr< Impl > impl_;
};

#endif