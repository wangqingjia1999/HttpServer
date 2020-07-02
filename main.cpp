#include <iostream>

#include "include/Server.hpp"
#include "include/Request.hpp"
#include "include/Response.hpp"

int main() 
{
    Server server;

    std::cout << "Start to listening" << std::endl;
    
    while(server.listen_at("localhost", 2334))
    {
        if(!server.receive_request()) 
            std::cout << "Error receive request" << std::endl;
        if(!server.parseRequest())
            std::cout << "Error parse request" << std::endl;
        if(!server.generate_response())
            std::cout << "Error generate response" << std::endl;
        if(!server.send_response())
            std::cout << "Error send response" << std::endl;
    }
}
