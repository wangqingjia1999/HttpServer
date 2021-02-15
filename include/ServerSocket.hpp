#pragma once

#include "Timer.hpp"
#include "Logger.hpp"
#include "ISocket.hpp"

#include <queue>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstring>
#include <stdexcept>

#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

/**
 * States of server socket.
 */
enum class Server_Socket_State 
{
    ERROR,          // Ready for sending responses
    WRITABLE,       // Ready for accepting requests
    READABLE,       // Error occurs
    UNINITIALIZED,  // Uninitialized 
};

/**
 * In:  response data in the form of std::string
 * Out: request data in the form of std::string
 */
class ServerSocket
{
public:
    ServerSocket();
    ~ServerSocket();

    ServerSocket(const ServerSocket&) = delete;
    ServerSocket& operator=(const ServerSocket&) = delete;

    ServerSocket(const ServerSocket&&) = delete;
    ServerSocket& operator=(const ServerSocket&&) = delete;
    
public:
    bool write_to(const std::string& data_string);
    bool write_to(const int peer_fd, const std::string& data_string);

    std::string read_from();
    std::string read_from(const int peer_fd);
public:
    /**
     * Initialize server socket.
     * 
     * @param[in] ip
     *      Ip address to bind to.
     * 
     * @param[in] port
     *      Port number.
     * 
     * @return 
     *      True if succeeds. 
     */
    bool initialize_server_socket(const std::string ip, const int port);

    /**
     * Listen at given ip:port.
     * 
     * @param  ip  
     *      Ip address string.
     * @param  port  
     *      Port number.
     * 
     * @retval Server_Socket_State::WRITABLE
     *      Ready for sending responses
     * 
     * @retval Server_Socket_State::READABLE
     *      Ready for receiving requests
     * 
     * @retval Server_Socket_State::ERROR
     *      Error occurs
     */
    Server_Socket_State listen_at(const std::string ip, const int port);

    std::vector<uint8_t>* get_receive_buffer();
    std::vector<uint8_t>* get_send_buffer();

    bool set_socket_non_blocking(const int socket_fd);

    int get_readable_fd() const;
public:
    static std::string generate_string_from_byte_stream(const std::vector<uint8_t>& byte_stream);

private:
    int readable_fd;
    Server_Socket_State server_socket_state;
    int listen_fd;

    std::vector< uint8_t > receive_buffer;
    std::vector< uint8_t > send_buffer;

    int epfd;
    epoll_event* triggered_events;

    /**
     * Contains all the file descriptors that are waiting for writing to
     */
    std::queue< int > send_fd_queue;
    bool has_finished_initialization;
};
