#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Uri.hpp"

#ifdef __linux__
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <error.h>
#endif

#include <memory>
#include <string>

struct Client::Impl
{
    #ifdef __linux__
    int clientSocket;
    int serverSocket;
    #endif

    std::shared_ptr< Message::Request > request = std::make_shared< Message::Request >();
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
};

// lifecylce management
Client::~Client() = default;
Client::Client() : impl_(new Impl)
{
}

Client::Client(const Client& other) noexcept
{
    *this = other;
}

Client& Client::operator=(const Client& other) noexcept
{
    if(this!=&other)
    {
        *impl_=*other.impl_;
    }
    return *this;
}

Client::Client(Client&&) noexcept = default;
Client& Client::operator=(Client&&) noexcept = default;

// public methods
bool Client::connectTo(const std::string host, const std::string port)
{
    #ifdef __linux__
    // set address that contains host and post
    struct addrinfo hints;
    struct addrinfo* result, *resultPtr;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;
    int addrResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
    if(addrResult != 0)
    {
        perror("getaddrinfo");
        return false;
    }

    impl_->clientSocket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);
    if(impl_->clientSocket == -1)
    {
        return false;
    }

    size_t addressLength = sizeof(struct addrinfo);
    resultPtr = result;
    int connectResult = connect(impl_->clientSocket, resultPtr->ai_addr, resultPtr->ai_addrlen);
    if(connectResult == -1)
    {
        perror("connect");
        return false;
    }
    
    return true;
    #endif
}

bool Client::sendRequest()
{
    #ifdef __linux__
    int sendResult = send(impl_->clientSocket, 
        impl_->request->getGeneratedRequestString().c_str(), 
        impl_->request->getGeneratedRequestString().size(), 
        0
    );
    if(sendResult == -1)
    {
        return false;
    }

    return true;
    #endif
}

bool Client::parseResponse()
{
    // TODO: add impl_ to memebers of request
    #ifdef _WIN32

    auto statusLineEndDelimiter = rawResponse.find("\r\n");

    std::string statusLine = rawResponse.substr(0, statusLineEndDelimiter);

    auto firstSpace = statusLine.find(" ");
    auto secondSpace = statusLine.find_last_of(" ");

    responsePtr->setProtocolVersion(statusLine.substr(0, firstSpace));
    responsePtr->setStatus(std::stoi(statusLine.substr(firstSpace, secondSpace)));

    auto headersEndDelimiter = rawResponse.find("\r\n\r\n");
    std::string headers = rawResponse.substr(statusLineEndDelimiter+2, headersEndDelimiter);

    // TODO: change this while loop to parseHeaders() function
    while (headers.find("\r\n") != std::string::npos)
    {
        std::string singleLineHeader = headers.substr(0, headers.find("\r\n"));

        auto nameEndDelimiter = singleLineHeader.find(":");
        std::string name = singleLineHeader.substr(0, nameEndDelimiter);
        std::string value = singleLineHeader.substr(nameEndDelimiter+1, singleLineHeader.find("\r\n"));

        if (value[0] == ' ') { value = value.substr(1); }
        
        responsePtr->addHeader(name, value);

        headers = headers.substr(headers.find("\r\n") + 2);
    }
    responsePtr->setBody(rawResponse.substr(headersEndDelimiter + 4));
    
    return responsePtr;
    #endif
    #ifdef __linux__

    #endif

    return true;
}

bool Client::generateRequest()
{
    return impl_->request->generateRequest();
}

bool Client::parseUri(const std::string uri)
{
    return impl_->request->parseUri(uri);
}

std::string Client::getGeneratedRequest()
{
    return impl_->request->getGeneratedRequestString();
}