#include "Server.hpp"


namespace
{
    #ifdef __WIN32
    // established socket connection
    SOCKET clientSocket;
 
    // server listen socket
    SOCKET serverListenSocket;
    #endif

    #ifdef __linux__
    int clientSocket = socket(AF_LOCAL, SOCK_STREAM, 0);
    int serverSocket = socket(AF_LOCAL, SOCK_STREAM, 0);
    #endif

    static const int bufferLength = 1024;

    char receiveBuffer[bufferLength] = { 0 };
}


struct Server::Impl
{
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

    serverListenSocket = socket(addressPtr->ai_family, addressPtr->ai_socktype, addressPtr->ai_protocol);
    if (serverListenSocket == INVALID_SOCKET)
    {
        WSACleanup();
        return false;
    }


    // bind socket
    auto bindResult = bind(serverListenSocket, addressPtr->ai_addr, (int)addressPtr->ai_addrlen);
    if (bindResult == SOCKET_ERROR)
    {
        freeaddrinfo(addressPtr);
        closesocket(serverListenSocket);
        WSACleanup();
        return false;
    }

    // free address info 
    freeaddrinfo(addressPtr);

    auto listenResult = listen(serverListenSocket, SOMAXCONN);
    if (listenResult == SOCKET_ERROR)
    {
        closesocket(serverListenSocket);
        WSACleanup();
        return false;
    }

    clientSocket = accept(serverListenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        closesocket(serverListenSocket);
        WSACleanup();
        return false;
    }

    closesocket(serverListenSocket);

    return true;
    #endif

    #ifdef __linux__
    struct sockaddr_in internetAddress;
    int addressLength;
    
    // create socket
    int socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFileDescriptor == -1)
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
    int bindResult = bind(socketFileDescriptor, (struct sockaddr*)&internetAddress, internetAddressLength);
    if(bindResult == -1)
    {
        return false;
    }

    // listen at given host and port
    int listenResult = listen(socketFileDescriptor, 4096);
    if(listenResult == -1)
    {
        return false;
    }

    // accept coming socket
    int acceptedSocket = accept(socketFileDescriptor, NULL, NULL);
    if(acceptedSocket == -1)
    {
        return false;
    }

    return true;
    #endif
}

bool Server::sendResponse(const std::string& data, const size_t size)
{
    #ifdef _WIN32
    auto sendResult = send(clientSocket, data.c_str(), size, 0);
    if (sendResult == SOCKET_ERROR)
    {
        return false;
    }
    return true;
    #endif

    #ifdef __linux__
    int sendResult = send(clientSocket, data.c_str(), size, 0);
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
    memset(receiveBuffer, 0, bufferLength);
    auto receiveResult = recv(clientSocket, receiveBuffer, bufferLength, 0);
    if (receiveResult == SOCKET_ERROR)
    {
        impl_->rawRequest = "";
        return false;
    }

    impl_->rawRequest = receiveBuffer;

    memset(receiveBuffer, 0, bufferLength);

    return true;
    #endif

    #ifdef __linux__
    int receiveResult = recv(clientSocket, receiveBuffer, bufferLength, 0);
    if(receiveResult == -1)
    {
        impl_->request->setRawRequest("");
        return false;
    }
    
    impl_->request->setRawRequest(receiveBuffer);

    memset(receiveBuffer, 0, bufferLength);

    return true;
    #endif
}

