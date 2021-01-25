#include "../../include/ServerSocket.hpp"
#include "../../include/Server.hpp"

#include <unistd.h>

int main()
{
    printf("Simple server runs in process %d\n", getpid());
    Server server;
    server.listen_at("0.0.0.0", 18135);
}