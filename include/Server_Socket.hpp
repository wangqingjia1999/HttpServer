#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "Timer.hpp"


#include <queue>
#include <mutex>
#include <chrono>
#include <iostream>
#include <stdexcept>


#ifdef __linux__
    // TODO: linux platform
#elif _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #include <WinSock2.h>
    #include <WS2tcpip.h>
#endif

class Server_Socket
{
public:
    Server_Socket();

    Server_Socket(const Server_Socket&) = delete;
    Server_Socket& operator=(const Server_Socket&) = delete;

    Server_Socket(const Server_Socket&&) = delete;
    Server_Socket& operator=(const Server_Socket&&) = delete;
    
    /**
     * @brief  Listen at given ip:port.
     * @param  ip  Ip address string.
     * @param  port  Port number.
     */
    void listen_at(const std::string ip, const int port);

    bool has_client_connection();

    void write_to(const std::string& data);

    std::string read_from(); 

private:
    void print_socket_error();
private:
    int listen_result = 0;
#ifdef _WIN32
    SOCKET server_listening_socket = INVALID_SOCKET;
    SOCKET client_socket = INVALID_SOCKET;

#elif __linux__
    
#endif

};

#endif