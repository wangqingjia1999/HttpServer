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
    /**
     * @brief  Virtual deconstructor to ensure proper cleanup.
     */
    virtual ~ISocket() {};

    /**
     * @brief  Write/Send data to peer_socket.
     * @param  peer_socket  Peer's socket.
     * @param  data  Data to be wrote/sent.
     * @param  data_length  Length, in bytes, of the data in data_buffer.
     */
    virtual void write_to(const int peer_socket, const char* data_buffer, int data_length) = 0;
    
    /**
     * @brief  Read data from peer_socket.
     * @param  peer_socket  Peer's socket. 
     * @param  data  Data to be read.
     * @param  data_length  Length, in bytes, of the data in data_buffer.
     */
    virtual void read_from(const int peer_socket, char* data_buffer, int data_length) = 0;
};


#endif