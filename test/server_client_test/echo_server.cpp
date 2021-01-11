#include "../../include/Server_Socket.hpp"

int main()
{
    Server_Socket server_socket;
    server_socket.listen_at("0.0.0.0", 18135);
}