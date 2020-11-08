#include <iostream>

#include "include/Server.hpp"

int main()
{
    Server server;
    server.listen_at("127.0.0.0", 2333);
}
