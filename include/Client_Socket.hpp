#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include "ISocket.hpp"

class Client_Socket : public ISocket
{
public:
    void connect_to() override;

    virtual void close_connection() override;

    virtual void send_to() override;

    virtual void receive_from() override;

};


#endif