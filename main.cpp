#include <iostream>

#include "include/Server.hpp"
#include "include/Request.hpp"
#include "include/Response.hpp"

int main() 
{
    Server server;

    std::cout << "Start to listening" << std::endl;
    
    while(server.listenAt("localhost", 2333))
    {
        if(!server.receiveRequest()) 
            std::cout << "Error receive request" << std::endl;
        if(!server.parseRequest())
            std::cout << "Error parse request" << std::endl;
        if(!server.generateResponse())
            std::cout << "Error generate response" << std::endl;
        if(!server.sendResponse())
            std::cout << "Error send response" << std::endl;
    }
}
