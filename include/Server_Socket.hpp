#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "ISocket.hpp"
#include "Timer.hpp"

#include <chrono>
#include <queue>

#ifdef __linux__
    // TODO: linux platform
#elif _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #include <WinSock2.h>
#endif

enum class server_status {
    READY,
    LISTENING,
    LISTENING_TIMEOUT,
    ERROR_OCCURS
};
class Server_Socket : public ISocket
{
public:
    /**
     * @brief  Listen at given ip:port.
     * @param  ip  Ip address string.
     * @param  port  Port number.
     * @param  timeout_microseconds  Listen for timeout_microseconds microseconds.
     */
    void listen_at( const std::string ip, const int port, const long timeout_microseconds = -1 );

    virtual void write_to(const int peer_socket, const char* data_buffer, int data_length) override;

    virtual void read_from(const int peer_socket, char* data_buffer, int data_length) override;
    
    server_status get_current_server_status();

private:
    void add_socket_to_read_fds( SOCKET socket );
    void add_socket_to_write_fds( SOCKET socket );
    void remove_socket_from_read_fds( SOCKET socket );
    void remove_socket_from_write_fds( SOCKET socket );

private:
    std::queue< server_status > server_status_recorder;
#ifdef _WIN32
    SOCKET server_listening_socket = INVALID_SOCKET;
    fd_set read_fds;
    fd_set write_fds;
    timeval listen_time_out_duration;
#elif __linux__
    
#endif

};

#endif