#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "Timer.hpp"
#include "ISocket.hpp"

#include <queue>
#include <mutex>
#include <vector>
#include <chrono>
#include <cstring>
#include <stdexcept>

#include <unistd.h>
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

class Server_Socket // FIXME: after finishing implementation, I will define the interface for ISocket
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
    /**
     * Universal interface for sending data.
     */
    bool write_to(const std::string& data_string);

    /**
     * Internal implementation for sending data.
     */
    bool write_to(const std::vector<uint8_t>& data, const int data_size);
    
    std::vector<uint8_t>* read_from(const int peer_fd);

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

    void fill_send_buffer(const std::string& data_string);
    void fill_send_buffer(const std::vector<uint8_t>& data_stream);

    void print_receive_buffer();

public:
    static std::string generate_string_from_byte_stream(const std::vector<uint8_t>& byte_stream);

private:
    Server_Socket_State server_socket_state;
    int listen_fd;

    std::vector< uint8_t > receive_buffer;
    std::vector< uint8_t > send_buffer;

    int epfd;
    epoll_event* triggered_events;

    // Contains all the file descriptors that 
    // are waiting for writing to
    std::queue< int > send_fd_queue;
    bool has_finished_initialization;
};

#endif