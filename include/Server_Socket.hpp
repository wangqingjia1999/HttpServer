#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "ISocket.hpp"
#include "Timer.hpp"

#include <chrono>
#include <queue>
#include <mutex>

#ifdef __linux__
    // TODO: linux platform
#elif _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #include <WinSock2.h>
    #include <WS2tcpip.h>
#endif

enum class Server_Status {
    CLOSED,
    LISTENING
};
class Server_Socket : public ISocket
{
public:
    Server_Socket();

    /**
     * @brief  Listen at given ip:port.
     * @param  ip  Ip address string.
     * @param  port  Port number.
     * @param  timeout_microseconds  Listen for timeout_microseconds microseconds.
     */
    void listen_at( const std::string ip, const int port, const long timeout_microseconds = -1 );
    
    void stop_listening();

    virtual void write_to(const int peer_socket, const char* data_buffer, int data_length) override;

    virtual void read_from(const int peer_socket, char* data_buffer, int data_length) override;
    
    Server_Status get_current_server_status();
    
private:
    bool is_server_listening();
    void set_server_status( Server_Status new_status );
    void print_socket_error();
private:
    Server_Status server_status;
    std::mutex server_status_mutex;

    std::mutex is_listening_mutex;
    bool is_listening = true;

#ifdef _WIN32
    SOCKET server_listening_socket = INVALID_SOCKET;

#elif __linux__
    
#endif

};

#endif