#include "../../include/ServerSocket.hpp"
#include "../../include/Server.hpp"

#include <unistd.h>

int main()
{
    Logger::info("Simple server runs in process " + std::to_string(getpid()));
    Server server;
    server.listen_at("0.0.0.0", 80);
}