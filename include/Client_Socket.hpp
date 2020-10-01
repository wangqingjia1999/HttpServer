#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include "ISocket.hpp"

#include <vector>

class Client_Socket : public ISocket
{
public:
    /**
     * @brief  Connect to server with the identity of ip:port.
     * @param  ip  Ip address of server.
     * @param  port  Port of server.
     */
    void connect_to(const std::string ip, const int port);

    virtual void write_to(const int peer_socket, const std::vector< uint8_t >& data) override;

    virtual void read_from(const int peer_socket, const std::vector< uint8_t >& data) override;

};


#endif