#include "Client_Socket.hpp"

#include <stdexcept>
#include <iostream>

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

    client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(client_socket == INVALID_SOCKET)
    {
        WSACleanup();
        throw std::runtime_error("Can not create client socket");
    }

    sockaddr_in client_socket_info;
    client_socket_info.sin_family = AF_INET;
    client_socket_info.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    client_socket_info.sin_port = htons(port);

    auto connect_result = connect(client_socket, (sockaddr*)&client_socket_info, sizeof(client_socket_info));

    if(connect_result == INVALID_SOCKET)
    {
    #ifdef _WIN32
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
        closesocket(client_socket);
        WSACleanup();
        std::wstring error_string(error_info);
        throw std::runtime_error( std::string( error_string.begin(), error_string.end() ) );
    #endif
    }

    // Add read/write

    closesocket(client_socket);
    WSACleanup();
    return;

#endif
}

void Client_Socket::write_to(const int peer_socket, const char* data_buffer, int data_length)
{

}

void Client_Socket::read_from(const int peer_socket, char* data_buffer, int data_length)
{
    
}