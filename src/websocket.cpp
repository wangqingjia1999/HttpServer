#include "websocket.hpp"
#include "logger.hpp"
#include "status_handler.hpp"
#include "base64.hpp"
#include "sha1.hpp"

// helpers 
namespace
{
    /**
     * Currently, it only supports websocket version 13.
     */
    const std::string WEBSOCKET_VERSION = "13";

    /**
     * The base64-encoded key length of "Sec-WebSocket-Key".
     */
    constexpr size_t WEBSOCKET_KEY_LENGTH = 16;

    /**
     * ...
     */
    const std::string WEBSOCKET_KEY = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    /**
     * 0x80 (hexadecimal) = 1000 0000b (binary)
     * Fin is the bit to set in the first octet of the last WebSocket frame in a message.
     */
    constexpr uint8_t FIN = 0x80;

    /**
     * 0x80 (hexadecimal) = 1000 0000b (binary) 
     * MASK is the bit to set in the second octet of a WebSocket frame to indicate that the 
     * payload of the frame is masked, and a masking key is included.
     */
    constexpr uint8_t MASK = 0x80;

    /**
     * 0x00 = 0000 0000b
     * Opcode for a continuation frame.
     */
    constexpr uint8_t OPCODE_CONTINUATION = 0x00;

    /**
     * 0x01 = 0000 0001b
     * OPcode for a text frame.
     */
    constexpr uint8_t OPCODE_TEXT = 0x01;

    /**
     * 0x02 = 0000 0010b
     * Opcode for a binary frame.
     */
    constexpr uint8_t OPCODE_BINARY = 0x02;

    /**
     * 0x08 = 0000 1000b
     * Opcode for a close frame.
     */
    constexpr uint8_t OPCODE_CLOSE = 0x08;

    /**
     * 0x09 = 0000 1001b
     * Opcode for a ping frame.
     */
    constexpr uint8_t OPCODE_PING = 0x09;

    /**
     * 0x0A = 0000 1010b
     * Opcode for a pong frame.
     */
    constexpr uint8_t OPCODE_PONG = 0x0A;

    /**
     * Maximum length of a control frame payload data.
     */
    constexpr size_t MAX_CONTROL_FRAME_DATA_LENGTH = 125;

    enum class fragmented_message_type
    {
        /**
         * We're not sending/receving any message.
         */
        none,

        /**
         * We're sending/receiving a text message.
         */
        text,

        /**
         * We're sending/receiving a binary message.
         */
        binary,
    };

    enum class type_of_event
    {
        /**
         * Default type used if an event's type is not yet set up.
         */
        unknown,

        /**
         * A text message was received.
         */
        text,

        /**
         * A binary message was received.
         */
        binary,
        
        /**
         * A ping was received.
         */
        ping,
        
        /**
         * A pong was received.
         */
        pong,
        
        /**
         * The WebSocket was closed.
         */
        close
    };
    
    /**
     * Default received message type.
     */
    type_of_event type = type_of_event::unknown;

    /**
     * Data payload received.
     */
    std::string data;

    /**
     * Status code from the received close frame.
     */
    unsigned int close_code;
}

struct websocket::Impl
{
    std::shared_ptr< Message::Request > request;
    std::shared_ptr< Message::Response > response;
};

websocket::~websocket() noexcept
{
}
websocket::websocket(
    std::shared_ptr< Message::Request > request,
    std::shared_ptr< Message::Response > response
) : impl_(new Impl)
{
    impl_->request = request;
    impl_->response = response;
}


// move semantic
websocket::websocket(websocket&&) noexcept = default;
websocket& websocket::operator=(websocket&&) noexcept = default;

// public methods
bool websocket::generate_websocket_request()
{
    impl_->response->set_protocol_version("HTTP/1.1");
    impl_->response->set_status(101);
    return true;
}

bool websocket::parse_websocket_request()
{    
    if(
        impl_->request->get_method() != "GET"
        || !impl_->request->has_header("Connection")
        || !impl_->request->has_header("Upgrade")
        || !impl_->request->has_header("Sec-WebSocket-Key")
        || !impl_->request->has_header("Sec-WebSocket-Version")
    )
    {
        status_handler::handle_status_code(impl_->response, 400);
        return false;
    }

    return true;
}

std::string websocket::generate_sec_websocket_key()
{
    return base64::encode_hex_string(
        sha1::sha1_encrypt(
            impl_->request->get_header("Sec-WebSocket-Key") + WEBSOCKET_KEY
        )
    );
}