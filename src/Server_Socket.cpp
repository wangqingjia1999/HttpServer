#include "Server_Socket.hpp"

#include <stdexcept>

#ifdef _WIN32
    #pragma comment(lib, "Ws2_32.lib")
#endif

void Server_Socket::listen_at( const std::string ip, const int port ) 
{
    SOCKET listen_socket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if(listen_socket == INVALID_SOCKET)
    {
        throw std::invalid_argument("Can not create a new socket");
        return;
    }

    sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.S_un.S_addr = inet_addr( ip.c_str() );
    socket_address.sin_port = htons(port);

    int bind_result = bind(
        listen_socket, 
        (sockaddr*)&socket_address, 
        sizeof(socket_address)
    );

    if(listen_socket == INVALID_SOCKET)
    {
        throw std::invalid_argument("Can not bind a socket");
        return;
    }

    int listen_result = listen(listen_socket, 999);
}


void Server_Socket::write_to(const int peer_socket, const std::vector< uint8_t >& data)
{

}

void Server_Socket::read_from(const int peer_socket, const std::vector< uint8_t >& data)
{

}