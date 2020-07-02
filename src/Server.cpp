#include "Server.hpp"

struct Server::Impl
{
    #ifdef __WIN32
    SOCKET client_socket;
    SOCKET server_socket;
    #endif

    #ifdef __linux__
    int client_socket;
    int server_socket;
    #endif

    static const int buffer_length = 1024;
    char received_buffer[buffer_length] = { 0 };

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

bool Server::listen_at(const std::string& host, const int port)
{
    #ifdef _WIN32
    // address info holds host address information
    struct addrinfo* addressPtr = nullptr;

    // Initialize Winsock
    WSADATA wsaData;
    auto initialize_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (initialize_result != 0)
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
    initialize_result = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &addressPtr);
    if (initialize_result != 0)
    {
        WSACleanup();
        return false;
    }

    impl_->server_socket = socket(addressPtr->ai_family, addressPtr->ai_socktype, addressPtr->ai_protocol);
    if (impl_->server_socket == INVALID_SOCKET)
    {
        WSACleanup();
        return false;
    }


    // bind socket
    auto bind_result = bind(impl_->server_socket, addressPtr->ai_addr, (int)addressPtr->ai_addrlen);
    if (bind_result == SOCKET_ERROR)
    {
        freeaddrinfo(addressPtr);
        closesocket(impl_->server_socket);
        WSACleanup();
        return false;
    }

    // free address info 
    freeaddrinfo(addressPtr);

    auto listen_result = listen(impl_->server_socket, SOMAXCONN);
    if (listen_result == SOCKET_ERROR)
    {
        closesocket(impl_->server_socket);
        WSACleanup();
        return false;
    }

    client_socket = accept(impl_->server_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET)
    {
        closesocket(impl_->server_socket);
        WSACleanup();
        return false;
    }

    closesocket(impl_->server_socket);

    return true;
    #endif

    #ifdef __linux__
    // IPv4 socket address
    struct sockaddr_in ipv4_address;
    
    // create socket
    impl_->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(impl_->server_socket == -1)
    {
        return false;
    }

    // set address info
    memset(&ipv4_address, 0 , sizeof(ipv4_address));
    ipv4_address.sin_family = AF_INET;
    ipv4_address.sin_addr.s_addr = inet_addr(host.c_str());
    ipv4_address.sin_port = htons(port);
    int ipv4_address_length = sizeof(ipv4_address);

    // bind address to socket
    int bind_result = bind(impl_->server_socket, (struct sockaddr*)&ipv4_address, ipv4_address_length);
    if(bind_result == -1)
    {
        return false;
    }

    // listen at given host and port
    int listen_result = listen(impl_->server_socket, 4096);
    if(listen_result == -1)
    {
        return false;
    }

    return true;
    #endif
}

bool Server::send_response()
{
    #ifdef _WIN32
    int sendResult = send(
        impl_->client_socket, 
        impl_->response->get_response_message().c_str(), 
        impl_->response->get_response_length(), 
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
        impl_->client_socket, 
        impl_->response->get_response_message().c_str(), 
        impl_->response->get_response_length(), 
        0
    );

    if(sendResult == -1)
    {
        return false;
    }
    return true;
    #endif
}

bool Server::receive_request()
{
    #ifdef _WIN32
    memset(impl_->received_buffer, 0, impl_->buffer_length);
    auto receive_result = recv(impl_->client_socket, impl_->received_buffer, impl_->buffer_length, 0);
    if (receive_result == SOCKET_ERROR)
    {
        impl_->raw_request = "";
        return false;
    }

    impl_->raw_request = impl_->received_buffer;

    memset(impl_->received_buffer, 0, impl_->buffer_length);

    return true;
    #endif

    #ifdef __linux__
    int receive_result = recv(impl_->client_socket, impl_->received_buffer, impl_->buffer_length, 0);
    if(receive_result == 0 || receive_result == -1)
    {
        impl_->request->set_raw_request("");
        return false;
    }
    
    if(!impl_->request->set_raw_request(impl_->received_buffer))
    {
        return false;
    }

    memset(impl_->received_buffer, 0, impl_->buffer_length);

    return true;
    #endif
}

bool Server::parse_request()
{
    return impl_->request->parse_raw_request();
}

bool Server::generate_response()
{
    return impl_->response->generate_response();
}

std::string Server::get_raw_request()
{
    return impl_->request->get_raw_request();
}

int Server::get_client_fd()
{
    return impl_->client_socket;
}

int Server::get_server_fd()
{
    return impl_->server_socket;
}