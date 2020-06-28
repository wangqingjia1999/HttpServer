#include <iostream>

#include "include/Server.hpp"
#include "include/Request.hpp"
#include "include/Response.hpp"

int main() 
{
    Server server;
    Message::Request request;
    Message::Response response;

    std::cout << "I'm listening" << std::endl;

    while(server.listenAt("localhost", 54321))
    {
        std::cout << "I'm listening" << std::endl;

        server.receiveRequest();
        request.parseRawRequest();

        response.setContent(request.getRequestUri());
        
        response.setStatus(200);
        response.addHeader("Accept-Ranges", "bytes");
        response.addHeader("Date", "Mon, 27 Jul 2009 12:28:53 GMT");
        response.generateResponse();
        server.sendResponse(response.getResponseMessage(), response.getResponseMessageLength());        
    }
}
