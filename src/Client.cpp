#include "Client.hpp"

struct Client::Impl
{
    // TODO:
    // write private members & methods of Client class here.
};

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

#ifdef _WIN32
std::shared_ptr<Message::Response> HTTP::Client::parseResponse(const std::string& rawResponse)
{
    std::shared_ptr< Message::Response > responsePtr{ new Message::Response };

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
}

#elif defined(__linux__)

#endif