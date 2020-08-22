#include <sys/socket.h>

#include "WebSocket.hpp"
#include "Logger.hpp"
#include "Status_Handler.hpp"
#include "Base64.hpp"
#include "SHA1.hpp"

// helpers 
namespace
{
    /**
     * Currently, it only supports WebSocket version 13.
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

    /**
     * Check whether the frame is masked.
     * 
     * @param[in] frame
     *      Frame string.
     * @return 
     *      true if it's masked;
     *      false if not.
     */
    bool is_masked_frame();

    /** 
     * Check whether the frame's FIN bit is set.
     * 
     * @return
     *      true if FIN is set;
     *      false if not.
     */
    bool is_fin_frame();

    /**
     * Send data in either text or binary format.
     * 
     * @param[in] file_descriptor
     *      File descriptor of receiver.
     * @param[in] data
     *      Data in the form of text or binary.
     * @return 
     *      true if succeeds;
     *      false if fails.
     */
    bool send_data(const int file_descriptor);
}

struct WebSocket::Impl
{
    std::shared_ptr< Message::Request > request;
    std::shared_ptr< Message::Response > response;
};

WebSocket::~WebSocket() noexcept
{
}
WebSocket::WebSocket() : impl_(new Impl)
{
}

WebSocket::WebSocket(
    std::shared_ptr< Message::Request > request,
    std::shared_ptr< Message::Response > response
) : impl_(new Impl)
{
    impl_->request = request;
    impl_->response = response;
}


// move semantic
WebSocket::WebSocket(WebSocket&&) noexcept = default;
WebSocket& WebSocket::operator=(WebSocket&&) noexcept = default;

// public methods
bool WebSocket::generate_websocket_request()
{
    impl_->response->set_protocol_version("HTTP/1.1");
    impl_->response->set_status(101);
    return true;
}

bool WebSocket::parse_websocket_request()
{    
    if(
        impl_->request->get_request_method() != "GET"
        || !impl_->request->has_header("Connection")
        || !impl_->request->has_header("Upgrade")
        || !impl_->request->has_header("Sec-WebSocket-Key")
        || !impl_->request->has_header("Sec-WebSocket-Version")
    )
    {
        Status_Handler::handle_status_code(impl_->response, 400);
        return false;
    }

    return true;
}

std::string WebSocket::generate_sec_websocket_key()
{
    return Base64::encode_hex_string(
        SHA1::sha1_encrypt(
            impl_->request->get_header("Sec-WebSocket-Key") + WEBSOCKET_KEY
        )
    );
}

std::string WebSocket::generate_sec_websocket_key(const std::string& key_string)
{
    return Base64::encode_hex_string(
        SHA1::sha1_encrypt(
            key_string + WEBSOCKET_KEY
        )
    );
}