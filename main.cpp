#include <iostream>

#include "include/Server.hpp"
#include "include/Request.hpp"
#include "include/Response.hpp"

int main() 
{
    Server server;

    while(server.listenAt("localhost", 4321))
    {
        server.receiveRequest();
        server.parseRequest();
        server.generateResponse();
        server.sendResponse();
    }
}
