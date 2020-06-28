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
        std::string rawRequest = server.receiveRequest();

        request.parseRawRequest(rawRequest);

        response.setContent(request.getRequestUri());
        
        response.setStatusCode(200);
        response.setReasonPhrase("OK");
        response.addHeader("Accept-Ranges", "bytes");
        response.addHeader("Date", "Mon, 27 Jul 2009 12:28:53 GMT");
        response.generateResponse();
        server.sendResponse(response.getResponseMessage(), response.getResponseMessageLength());        
    }
}
