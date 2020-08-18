#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Uri.hpp"

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <error.h>

#include <memory>
#include <string>

struct Client::Impl
{
    int clientSocket;
    int serverSocket;

    std::shared_ptr< Message::Request > request = std::make_shared< Message::Request >();
    std::shared_ptr< Message::Response > response = std::make_shared< Message::Response >();
};

// lifecylce management
Client::~Client() = default;
Client::Client() : impl_(new Impl)
{
}

Client::Client(const Client& other)
{
    *this = other;
}
Client& Client::operator=(const Client& other)
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
bool Client::connect_to()
{
    // set address that contains host and post
    struct addrinfo hints;
    struct addrinfo* result, *resultPtr;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = 0;
    
    int addrResult = getaddrinfo(impl_->request->get_host().c_str(), 
        impl_->request->get_port().c_str(), 
        &hints, 
        &result
    );

    if(addrResult != 0)
    {
        perror("getaddrinfo");
        return false;
    }

    impl_->serverSocket = socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol);
    if(impl_->serverSocket == -1)
    {
        return false;
    }

    resultPtr = result;
    int connectResult = connect(impl_->serverSocket, resultPtr->ai_addr, resultPtr->ai_addrlen);
    if(connectResult == -1)
    {
        perror("connect");
        return false;
    }
    
    return true;
}

bool Client::send_request()
{
    int sendResult = send(impl_->serverSocket, 
        impl_->request->get_generated_request().c_str(), 
        impl_->request->get_generated_request().size(), 
        0
    );
    if(sendResult == -1)
    {
        perror("send");
        return false;
    }

    return true;
}

bool Client::parse_response()
{
    

    return true;
}

bool Client::generate_request()
{
    return impl_->request->generate_request();
}

bool Client::parse_uri(const std::string uri)
{
    return impl_->request->parse_uri(uri);
}

std::string Client::get_generated_request()
{
    return impl_->request->get_generated_request();
}