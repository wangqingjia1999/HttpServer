#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "Timer.hpp"
#include "ISocket.hpp"

#include <queue>
#include <mutex>
#include <vector>
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
    bool write_to(const int peer_fd, const std::vector<uint8_t>& data, const int data_size) override;
    std::vector<uint8_t>* read_from(const int peer_fd) override;

public:
    /**
     * @brief  Listen at given ip:port.
     * @param  ip  Ip address string.
     * @param  port  Port number.
     */
    void listen_at(const std::string ip, const int port);

    std::vector<uint8_t>* get_receive_buffer();
    std::vector<uint8_t>* get_send_buffer();

    void fill_send_buffer(const std::string& data_string);
    void fill_send_buffer(const std::vector<uint8_t>& data_stream);

    void print_receive_buffer();
private:
    void print_socket_error();

private:
    int listen_fd;

    std::vector<uint8_t> receive_buffer;
    std::vector<uint8_t> send_buffer;

#ifdef _WIN32
    SOCKET server_listening_socket;
    SOCKET client_socket;
#elif __linux__
    int epfd;
    epoll_event server_epoll_event;
#endif

};

#endif