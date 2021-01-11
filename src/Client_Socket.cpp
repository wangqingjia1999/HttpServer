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
#ifdef _WIN32
    WSADATA wsa_data = {0};
    // Request WinSock version 2.2 
    auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if(result != NO_ERROR)
    {
        WSACleanup();
        throw std::runtime_error("Can not initialize WSADATA");
    }
    client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client_fd == INVALID_SOCKET)
    {
        WSACleanup();
        throw std::runtime_error("Can not create client socket");
    }
    if(client_fd == -1)
    {
        perror("socket");
        close(client_fd);
        return;
    }

    sockaddr_in server_socket;
    server_socket.sin_family = AF_INET;
    server_socket.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    server_socket.sin_port = htons(port);
    
    std::cout << "I'm trying to connect" << std::endl;
    
    auto connect_result = connect(client_fd, (sockaddr*)&server_socket, sizeof(server_socket));
    
    if(connect_result == INVALID_SOCKET)
    {
        wchar_t* error_info = nullptr;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
            nullptr, 
            WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&error_info, 
            0, 
            nullptr
        );
        closesocket(client_fd);
        WSACleanup();
        std::wstring error_string(error_info);
        throw std::runtime_error( std::string( error_string.begin(), error_string.end() ) );
        perror("connect");
        close(client_fd);
    }
#elif __linux__
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
#endif
}

bool Client_Socket::write_to(const int peer_fd, const std::vector<uint8_t>& data, const int data_size)
{
    char local_send_buffer[8192];
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
    std::cout << receive_buffer_string;

    return &receive_buffer;
}

void Client_Socket::close_connection()
{
#ifdef _WIN32
    closesocket(client_fd);
    WSACleanup();
#elif __linux__
    if(close(client_fd))
        fprintf(stderr, "Cannot tear down client socket fd.");
#endif
    return;
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