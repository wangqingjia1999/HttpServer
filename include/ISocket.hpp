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
     * Virtual deconstructor to ensure proper cleanup.
     */
    virtual ~ISocket() {};

    /**
     * Write/Send data to peer_socket.
     * 
     * @param[in]  peer_socket  
     *      Peer's socket.
     * 
     * @param[in]  data
     *      Data to be wrote/sent.
     * 
     * @param[in]  data_length  
     *      Length, in bytes, of the data in data_buffer.
     */
    virtual size_t write_to(const int peer_socket, const char* data_buffer, const int data_size) = 0;
    
    /**
     * Read data from peer_socket.
     * 
     * @param[in]  peer_socket  
     *      Peer's socket. 
     * 
     * @param[in]  data  
     *      Data to be read.
     * 
     * @param[in]  data_length  
     *      Length, in bytes, of the data in data_buffer.
     */
    virtual size_t read_from(const int peer_socket, char* data_buffer, const int data_size) = 0;
};


#endif