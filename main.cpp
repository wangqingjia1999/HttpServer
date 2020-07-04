#include <iostream>

#include "include/Server.hpp"
#include "include/Request.hpp"
#include "include/Response.hpp"

int main() 
{
    Server server;
    std::cout << "I'm listening :) " << std::endl;
    server.listen_at("127.0.0.1", 2333);
}
