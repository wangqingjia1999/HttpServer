#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "Timer.hpp"
#include "ISocket.hpp"

#include <queue>
#include <mutex>
#include <chrono>
#include <cstring>
#include <iostream>
#include <stdexcept>

#ifdef __linux__
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/types.h>
    #include <sys/epoll.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/ip.h>
#elif _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #include <WinSock2.h>
    #include <WS2tcpip.h>
#endif

class Server_Socket : public ISocket
{
public:
    Server_Socket();
    ~Server_Socket();

    Server_Socket(const Server_Socket&) = delete;
    Server_Socket& operator=(const Server_Socket&) = delete;

    Server_Socket(const Server_Socket&&) = delete;
    Server_Socket& operator=(const Server_Socket&&) = delete;
    
    // overrides
public:
    size_t write_to(const int peer_socket, const char* data_buffer, const int data_size) override;

    size_t read_from(const int peer_socket, char* data_buffer,  const int data_size) override;

public:
    /**
     * @brief  Listen at given ip:port.
     * @param  ip  Ip address string.
     * @param  port  Port number.
     */
    void listen_at(const std::string ip, const int port);

private:
    void print_socket_error();
private:
    int listen_fd;
#define RECEIVE_BUFFER_SIZE 1024
    char receive_buffer[RECEIVE_BUFFER_SIZE];

#define SEND_BUFFER_SIZE 1024
    char send_buffer[SEND_BUFFER_SIZE];

#ifdef _WIN32
    SOCKET server_listening_socket;
    SOCKET client_socket;
#elif __linux__
    int epfd;
    epoll_event server_epoll_event;
#endif

};

#endif