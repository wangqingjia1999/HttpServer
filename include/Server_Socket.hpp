#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "ISocket.hpp"

#ifdef __linux__

#elif _WIN32
    #include <WinSock2.h>
#endif

class Server_Socket : public ISocket
{
public:
    void listen_at( const std::string ip, const int port ) override;
    void close_connection() override;
    void send_to() override;
    void receive_from() override;
private:

};

#endif