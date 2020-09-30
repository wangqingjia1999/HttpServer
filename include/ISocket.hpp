#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <vector>
#include <string>

/**
 * Socket interface class.
 */
class ISocket
{
public:
    virtual ~ISocket() {} 
    /**
     * @brief  Let server listens at specific {ip address, port} pair.
     * @param  ip  Ip address.
     * @param  port  Port number.
     */
    virtual void listen_at( const std::string ip, const int port );

    /**
     * @brief  Let client connect to {ip address, port} pair.
     */
    virtual void connect_to();

    /**
     * @brief  Close the socket connection.
     */
    virtual void close_connection() = 0;

    /**
     * @brief  Send data to peer.
     */
    virtual void send_to() = 0;

    /**
     * @brief  Receive data from peer.
     */
    virtual void receive_from() = 0;

protected:
    ISocket();
};


#endif