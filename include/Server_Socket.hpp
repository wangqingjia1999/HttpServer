#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "ISocket.hpp"

#include <chrono>

#ifdef __linux__
    // TODO: linux platform
#elif _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #include <WinSock2.h>
#endif

class Server_Socket : public ISocket
{
public:
    /**
     * @brief  Listen at given ip:port.
     * @param  ip  Ip address string.
     * @param  port Port number.
     * @param  millisecond  Time in millisecond for listening.
     */
    void listen_at( const std::string ip, const int port, const long long millisecond = -1 );

    virtual void write_to(const int peer_socket, const char* data_buffer, int data_length) override;

    virtual void read_from(const int peer_socket, char* data_buffer, int data_length) override;

private:
#ifdef _WIN32
    SOCKET server_listening_socket = INVALID_SOCKET;
#elif __linux__
    
#endif

};

#endif