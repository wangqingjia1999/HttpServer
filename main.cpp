#include <iostream>

#include "include/Server.hpp"

int main()
{
    Server server;
    server.listen_at("192.168.204.128", 2333);
}
