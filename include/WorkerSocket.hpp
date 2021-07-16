#pragma once

#include <queue>
#include <mutex>
#include <vector>
#include <chrono>
#include <memory>
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

enum class Server_Socket_State 
{
    NEW_SOCKET,
    ERROR_SOCKET,
    UNKNOWN_SOCKET
};

/**
 * In:  response data in the form of std::string
 * Out: request data in the form of std::string
 */
class WorkerSocket
{
public:
    WorkerSocket();
    ~WorkerSocket();

    WorkerSocket(const WorkerSocket&) = delete;
    WorkerSocket& operator=(const WorkerSocket&) = delete;

    WorkerSocket(const WorkerSocket&&) = delete;
    WorkerSocket& operator=(const WorkerSocket&&) = delete;

public:
    bool write_to(const int client_socket, const std::string& data_string);
    std::string read_from(const int client_socket);

    std::shared_ptr< std::vector<uint8_t> > get_receive_buffer();
    std::shared_ptr< std::vector<uint8_t> > get_send_buffer();
    
    int get_readable_fd();

private:
    int m_listening_fd;
    int m_listening_port;
    std::string m_listening_ip;

    int m_epfd;
    int m_readable_fd;
    epoll_event* m_triggered_events;
    std::vector< uint8_t > m_send_buffer;
    Server_Socket_State m_server_socket_state;
    std::vector< uint8_t > m_receive_buffer;
};
