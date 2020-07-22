#include <iostream>

#include "include/Server.hpp"

int main()
{
    Server server;
    std::cout << "I'm listening :) " << std::endl;
    // here 192.168.148.128 is my virtual machine's IPv4 address.
    server.listen_at("192.168.32.128", 2333);
}
