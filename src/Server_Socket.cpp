#include "Server_Socket.hpp"

#include <stdexcept>

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

}
