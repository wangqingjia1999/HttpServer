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
    /**
     * @brief  Listen at given ip:port.
     * @param  ip  Ip address string.
     * @param  port Port number.
     */
    void listen_at( const std::string ip, const int port );

    virtual void write_to(const int peer_socket, const std::vector< uint8_t >& data) override;

    virtual void read_from(const int peer_socket, const std::vector< uint8_t >& data) override;

private:

};

#endif