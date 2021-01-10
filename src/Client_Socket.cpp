#include "Client_Socket.hpp"

#include <stdexcept>
#include <iostream>

Client_Socket::Client_Socket()
    : client_fd(-1)
{

}

Client_Socket::~Client_Socket()
{

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
        close(client_fd);
        return;
    }

    printf("Client: connects to server");
#endif
}

size_t Client_Socket::write_to(const int peer_socket, const char* data_buffer, const int data_size)
{
    return send(peer_socket, data_buffer, data_size, 0);
}

size_t Client_Socket::read_from(const int peer_socket, char* data_buffer, const int data_size)
{
    return recv(peer_socket, data_buffer, data_size, 0);
}

void Client_Socket::close_connection()
{
#ifdef _WIN32
    closesocket(client_fd);
    WSACleanup();
#elif __linux__

#endif
    return;
}