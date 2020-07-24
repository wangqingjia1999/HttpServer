#include <iostream>

#include "include/Server.hpp"

int main()
{
    Server server;
    // here 192.168.229.128 is my virtual machine's IPv4 address.
    // type "ip addr" in command line to see your ip :)
    server.listen_at("192.168.229.128", 2333);
}
