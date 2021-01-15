#include "Client_Socket.hpp"

#include <stdexcept>
#include <iostream>

Client_Socket::Client_Socket()
    : client_fd(-1)
{

}

Client_Socket::~Client_Socket()
{
    close_connection();
}

void Client_Socket::connect_to( const std::string ip, const int port )
{
    client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(client_fd == -1)
    {
        perror("socket");
        return;
    }

    sockaddr_in server_socket;
    server_socket.sin_family = AF_INET;
    server_socket.sin_addr.s_addr = inet_addr(ip.c_str());
    server_socket.sin_port = htons(port);
 
    int connect_result = connect(client_fd, (sockaddr*)&server_socket, sizeof(server_socket));
    if(connect_result == -1)
    {
        perror("connect");
        return;
    }

    printf("Client: successfully connects to server\n");
}

bool Client_Socket::write_to(const int peer_fd, const std::vector<uint8_t>& data, const int data_size)
{
    if(!send_buffer.empty())
        send_buffer.clear();
    
    char local_send_buffer[8192] = { 0 };
    for(int i = 0; i < data_size; ++i)
        local_send_buffer[i] = (char)data[i];

    int send_result = send(peer_fd, local_send_buffer, data_size, 0);
    if(send_result == -1)
    {
        perror("send");
        return false;
    }
    else
    {
        return true;
    }
}

std::vector<uint8_t>* Client_Socket::read_from(const int peer_fd)
{
    if(!receive_buffer.empty())
        receive_buffer.clear();

    char local_receive_buffer[8192] = { 0 };
    
    ssize_t receive_result = recv(peer_fd, local_receive_buffer, sizeof(local_receive_buffer), 0);
    if(receive_result == -1)
    {
        perror("recv");
        return {};
    }

    for(int i = 0; i < sizeof(local_receive_buffer); ++i)
        receive_buffer.push_back( (uint8_t)local_receive_buffer[i] );

    std::string receive_buffer_string;
    for(const auto& byte : receive_buffer)
        receive_buffer_string += (char)byte;
    printf("Receive: %s\n", receive_buffer_string.c_str());

    return &receive_buffer;
}

void Client_Socket::close_connection()
{
    if(close(client_fd))
        fprintf(stderr, "Cannot tear down client socket fd.");
}

int Client_Socket::get_client_fd() const
{
    return client_fd;
}

void Client_Socket::fill_send_buffer(const std::string& data_string)
{
    for(int i = 0; i < data_string.size(); ++i)
        send_buffer.push_back( (uint8_t)data_string[i] );
}

std::vector<uint8_t>* Client_Socket::get_send_buffer()
{
    return &send_buffer;
}

std::vector<uint8_t>* Client_Socket::get_receive_buffer()
{
    return &receive_buffer;
}

bool Client_Socket::write_to()
{
    return write_to(
        client_fd,
        send_buffer,
        send_buffer.size()
    );
}

std::vector<uint8_t>* Client_Socket::read_from()
{
    return read_from(client_fd);
}