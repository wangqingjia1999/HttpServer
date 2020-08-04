#include <iostream>

#include "include/Server.hpp"

int main()
{
    Server server;
    server.listen_at("192.168.72.128", 2333);
}
