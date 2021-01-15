#include "../../include/Server_Socket.hpp"
#include "../../include/Server.hpp"

int main()
{
    Server server;
    server.listen_at("0.0.0.0", 18135);
}