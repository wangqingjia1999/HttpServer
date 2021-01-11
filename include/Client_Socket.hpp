#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include "ISocket.hpp"

#include <vector>

#ifdef _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #include <WinSock2.h>
#elif __linux__
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif

class Client_Socket : public ISocket
{
public:
    Client_Socket();
    ~Client_Socket();

public:
    /**
     * @brief  Connect to server with the identity of ip:port.
     * @param  ip  Ip address of server.
     * @param  port  Port of server.
     */
    void connect_to(const std::string ip, const int port);

    void close_connection();

    int get_client_fd() const;

    virtual size_t write_to(const int peer_socket, const char* data_buffer, const int data_size) override;

    virtual size_t read_from(const int peer_socket, char* data_buffer, const int data_size) override;
private:
    int client_fd;
#ifdef _WIN32
    SOCKET client_socket;
#endif
};


#endif