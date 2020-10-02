#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include "ISocket.hpp"

#include <vector>

#ifdef _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #include <WinSock2.h>
#endif

class Client_Socket : public ISocket
{
public:
    /**
     * @brief  Connect to server with the identity of ip:port.
     * @param  ip  Ip address of server.
     * @param  port  Port of server.
     */
    void connect_to(const std::string ip, const int port);

    virtual void write_to(const int peer_socket, const char* data_buffer, int data_length) override;

    virtual void read_from(const int peer_socket, char* data_buffer, int data_length) override;
private:
#ifdef _WIN32
    SOCKET client_socket = INVALID_SOCKET;
#endif
};


#endif