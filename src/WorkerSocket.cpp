#include "WorkerSocket.hpp"
#include "Logger.hpp"

#include <exception>

namespace
{
    constexpr int MAXIMUM_BUFFER_SIZE = 8192;
}

WorkerSocket::WorkerSocket()
    : m_readable_fd{ -1 },
      m_server_socket_state{ Server_Socket_State::UNKNOWN_SOCKET }
{
}

WorkerSocket::~WorkerSocket()
{
}

bool WorkerSocket::write_to(const int client_socket, const std::string& data_string)
{
    int send_result = send(client_socket, data_string.c_str(), data_string.size(), 0);
    
    if(send_result < 0)
    {
        Logger::error("send() error: " + std::string{ strerror(errno) });
        return false;
    }
    else if(send_result == 0)
    {
        return false;
    }

    return true;
}   

std::string WorkerSocket::read_from(const int client_socket)
{
    char local_receive_buffer[MAXIMUM_BUFFER_SIZE] = { 0 };
    ssize_t receive_result = 0;
    
    std::string local_receive_buffer_string;
    while( (receive_result = recv(client_socket, &local_receive_buffer, MAXIMUM_BUFFER_SIZE, MSG_DONTWAIT)) > 0)
        local_receive_buffer_string += std::string(local_receive_buffer);

    if(receive_result < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
    {
        return local_receive_buffer_string;
    }
    else
    {
        Logger::error("worker read() error " + std::string{ strerror(errno) });
        close(client_socket);
    }

    return local_receive_buffer_string;
}

std::shared_ptr< std::vector<uint8_t> > WorkerSocket::get_receive_buffer()
{
    return std::shared_ptr< std::vector<uint8_t> >(&m_receive_buffer);
}

std::shared_ptr< std::vector<uint8_t> > WorkerSocket::get_send_buffer()
{
    return std::shared_ptr< std::vector<uint8_t> >(&m_send_buffer);
}