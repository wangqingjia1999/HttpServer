#include "Server.hpp"

struct Server::Impl
{
    #ifdef __WIN32
    SOCKET clientSocket;
    SOCKET serverSocket;
    #endif

    #ifdef __linux__
    int clientSocket;
    int serverSocket;
    #endif

    static const int bufferLength = 1024;
    char receiveBuffer[bufferLength] = { 0 };

    std::shared_ptr< Message::Response> response = std::make_shared< Message::Response >();
    std::shared_ptr< Message::Request> request = std::make_shared< Message::Request >();
};

Server::~Server() = default;
Server::Server() : impl_(new Impl)
{
}

Server::Server(const Server& other) noexcept
{
    *this = other;
}

Server& Server::operator=(const Server& other) noexcept
{
    if(this!=&other)
    {
        *impl_=*other.impl_;
    }
    return *this;
}

Server::Server(Server&&) noexcept = default;
Server& Server::operator=(Server&&) noexcept = default;

bool Server::listenAt(const std::string& host, const int port)
{
    #ifdef _WIN32
    // address info holds host address information
    struct addrinfo* addressPtr = nullptr;

    // Initialize Winsock
    WSADATA wsaData;
    auto initializeResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (initializeResult != 0)
    {
        return false;
    }

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    initializeResult = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &addressPtr);
    if (initializeResult != 0)
    {
        WSACleanup();
        return false;
    }

    impl_->serverSocket = socket(addressPtr->ai_family, addressPtr->ai_socktype, addressPtr->ai_protocol);
    if (impl_->serverSocket == INVALID_SOCKET)
    {
        WSACleanup();
        return false;
    }


    // bind socket
    auto bindResult = bind(impl_->serverSocket, addressPtr->ai_addr, (int)addressPtr->ai_addrlen);
    if (bindResult == SOCKET_ERROR)
    {
        freeaddrinfo(addressPtr);
        closesocket(impl_->serverSocket);
        WSACleanup();
        return false;
    }

    // free address info 
    freeaddrinfo(addressPtr);

    auto listenResult = listen(impl_->serverSocket, SOMAXCONN);
    if (listenResult == SOCKET_ERROR)
    {
        closesocket(impl_->serverSocket);
        WSACleanup();
        return false;
    }

    clientSocket = accept(impl_->serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        closesocket(impl_->serverSocket);
        WSACleanup();
        return false;
    }

    closesocket(impl_->serverSocket);

    return true;
    #endif

    #ifdef __linux__
    struct sockaddr_in internetAddress;
    int addressLength;
    
    // create socket
    impl_->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(impl_->serverSocket == -1)
    {
        return false;
    }

    memset(&internetAddress, 0 , sizeof(internetAddress));

    // set address info
    internetAddress.sin_family = AF_INET;
    internetAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    internetAddress.sin_port = htons(port);
    int internetAddressLength = sizeof(internetAddress);

    // bind address to socket
    int bindResult = bind(impl_->serverSocket, (struct sockaddr*)&internetAddress, internetAddressLength);
    if(bindResult == -1)
    {
        return false;
    }

    // listen at given host and port
    int listenResult = listen(impl_->serverSocket, 4096);
    if(listenResult == -1)
    {
        return false;
    }

    // accept coming socket
    impl_-> clientSocket = accept(impl_->serverSocket, NULL, NULL);
    if(impl_->clientSocket == -1)
    {
        return false;
    }

    return true;
    #endif
}

bool Server::sendResponse()
{
    #ifdef _WIN32
    int sendResult = send(
        impl_->clientSocket, 
        impl_->response->getResponseMessage().c_str(), 
        impl_->response->getResponseMessageLength(), 
        0
    );
    if (sendResult == SOCKET_ERROR)
    {
        return false;
    }
    return true;
    #endif

    #ifdef __linux__
    int sendResult = send(
        impl_->clientSocket, 
        impl_->response->getResponseMessage().c_str(), 
        impl_->response->getResponseMessageLength(), 
        0
    );

    if(sendResult == -1)
    {
        return false;
    }
    return true;
    #endif
}

bool Server::receiveRequest()
{
    #ifdef _WIN32
    memset(impl_->receiveBuffer, 0, impl_->bufferLength);
    auto receiveResult = recv(impl_->clientSocket, impl_->receiveBuffer, impl_->bufferLength, 0);
    if (receiveResult == SOCKET_ERROR)
    {
        impl_->rawRequest = "";
        return false;
    }

    impl_->rawRequest = impl_->receiveBuffer;

    memset(impl_->receiveBuffer, 0, impl_->bufferLength);

    return true;
    #endif

    #ifdef __linux__
    int receiveResult = recv(impl_->clientSocket, impl_->receiveBuffer, impl_->bufferLength, 0);
    if(receiveResult == 0 || receiveResult == -1)
    {
        impl_->request->setRawRequest("");
        return false;
    }
    
    if(!impl_->request->setRawRequest(impl_->receiveBuffer))
    {
        return false;
    }

    memset(impl_->receiveBuffer, 0, impl_->bufferLength);

    return true;
    #endif
}

bool Server::parseRequest()
{
    return impl_->request->parseRawRequest();
}

bool Server::generateResponse()
{
    return impl_->response->generateResponse();
}

std::string Server::getRawRequest()
{
    return impl_->request->getRawRequest();
}