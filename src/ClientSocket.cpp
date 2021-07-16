#include "ClientSocket.hpp"

#include <stdexcept>
#include <iostream>

#include <cstring>
#include <errno.h>

ClientSocket::ClientSocket()
    : client_fd(-1)
{
}

ClientSocket::~ClientSocket()
{
    close_connection();
}

void ClientSocket::connect_to(const std::string ip, const int port)
{
    client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client_fd == -1)
    {
        Logger::error("call to socket() failed.");
        return;
    }

    sockaddr_in server_socket;
    server_socket.sin_family = AF_INET;
    server_socket.sin_addr.s_addr = inet_addr(ip.c_str());
    server_socket.sin_port = htons(port);
 
    int connect_result = connect(client_fd, (sockaddr*)&server_socket, sizeof(server_socket));
    if(connect_result == -1)
    {
        Logger::error("call to connect() failed");
        return;
    }

    Logger::info("client successfully connects to server.");
}

bool ClientSocket::write_to(const int peer_fd, const std::vector<uint8_t>& data, const int data_size)
{
    if(!send_buffer.empty())
        send_buffer.clear();
    
    char local_send_buffer[8192] = { 0 };
    for(int i = 0; i < data_size; ++i)
        local_send_buffer[i] = (char)data[i];

    int send_result = send(peer_fd, local_send_buffer, data_size, 0);
    if(send_result == -1)
    {
        Logger::error("call to send() failed.");
        return false;
    }
    else
    {
        return true;
    }
}

std::vector<uint8_t>* ClientSocket::read_from(const int peer_fd)
{
    if(!m_receive_buffer.empty())
        m_receive_buffer.clear();

    char local_receive_buffer[8192] = { 0 };
    
    ssize_t receive_result = recv(peer_fd, local_receive_buffer, sizeof(local_receive_buffer), 0);
    if(receive_result == -1)
    {
        Logger::error("call to recv() failed.");
        return {};
    }

    for(int i = 0; i < sizeof(local_receive_buffer); ++i)
        m_receive_buffer.push_back( (uint8_t)local_receive_buffer[i] );

    std::string receive_buffer_string;
    for(const auto& byte : m_receive_buffer)
        receive_buffer_string += (char)byte;
    Logger::info("receive: " + receive_buffer_string);

    return &m_receive_buffer;
}

void ClientSocket::close_connection()
{
    if(close(client_fd) < 0)
        Logger::warn("close() on client file descriptor error due to: " + std::string(strerror(errno)));
}

int ClientSocket::get_client_fd() const
{
    return client_fd;
}

void ClientSocket::fill_send_buffer(const std::string& data_string)
{
    for(int i = 0; i < data_string.size(); ++i)
        send_buffer.push_back( (uint8_t)data_string[i] );
}

std::vector<uint8_t>* ClientSocket::get_send_buffer()
{
    return &send_buffer;
}

std::vector<uint8_t>* ClientSocket::get_receive_buffer()
{
    return &m_receive_buffer;
}

bool ClientSocket::write_to()
{
    return write_to(
        client_fd,
        send_buffer,
        send_buffer.size()
    );
}

std::vector<uint8_t>* ClientSocket::read_from()
{
    return read_from(client_fd);
}