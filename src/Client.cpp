#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "URI.hpp"

#ifdef __linux__
    #include <sys/socket.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <error.h>
#elif _WIN32
    // link with Ws2_32.lib
    #pragma comment(lib,"Ws2_32.lib")
    #include <WinSock2.h>
#endif

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
    return false;
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