#include <sys/epoll.h>

#include "Server.hpp"
#include "websocket.hpp"
#include "logger.hpp"
#include <iostream>

struct Server::Impl
{
    int client_socket;
    int server_socket;

    static const int buffer_length = 1024;
    char received_buffer[buffer_length] = { 0 };

    // Response object for generating response
    std::shared_ptr< Message::Response> response = std::make_shared< Message::Response >();

    // Server side request object that is responsible for parseing coming request.
    std::shared_ptr< Message::Request> request = std::make_shared< Message::Request >();
};

Server::~Server()
{
}
Server::Server() : impl_(new Impl)
{
}

Server::Server(const Server& other) 
{
    *this = other;
}
Server& Server::operator=(const Server& other)
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
    // Note that the listen socket file descriptor remains unchanged
    int listen_result = listen(impl_->server_socket, 4096);
    if(listen_result == -1)
    {
        return false;
    }

    // create epoll fd
    int epfd = epoll_create(1024);
    if(epfd < 0)
    {
        return false;
    }

    // create epoll event
    struct epoll_event ev;
    ev.data.fd = impl_->server_socket;
    // monitor inputs
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, impl_->server_socket, &ev);

    struct epoll_event epoll_result[1024];

    // server core loop
    for (;;)
    {       
        int nums = epoll_wait(epfd, epoll_result, 1024, -1);
        switch (nums)
        {
        case 0:
            logger::record("Wait: time out");
            
            break;
        case -1:
            logger::record("Wait: error occurs");
            break;
        default:
            {
                int i;
                for(i=0; i < nums; ++i)
                {
                    int arrived_socket_fd = epoll_result[i].data.fd;
                    uint32_t arrived_socket_event = epoll_result[i].events;
                    
                    // If new client socket comes into the listening queue of server listening socket.
                    if(arrived_socket_fd == impl_->server_socket && (arrived_socket_event & EPOLLIN))
                    {
                        struct sockaddr_in client_socket_address;
                        socklen_t client_socket_length = sizeof(client_socket_address);

                        // accept new client socket
                        impl_->client_socket = accept(
                            impl_->server_socket, 
                            (struct sockaddr*)&client_socket_address, 
                            &client_socket_length 
                        );
                        if(impl_->client_socket < 0)
                        {
                            logger::record("Failed to accept client socket");
                            continue;
                        }
                        
                        std::string ip_string = inet_ntoa(client_socket_address.sin_addr);
                        std::string port_string = std::to_string((client_socket_address.sin_port));
                        logger::record("Accept request from: " + ip_string + ":" + port_string);
                    
                        // and add new client socket into epoll events list
                        ev.data.fd = impl_->client_socket;
                        ev.events = EPOLLIN;
                        epoll_ctl(epfd, EPOLL_CTL_ADD, impl_->client_socket, &ev);
                        continue;
                    }
                    else if (arrived_socket_fd != impl_->server_socket) // If fds(exclude listen socket) triggered events
                    {
                        if(arrived_socket_event & EPOLLIN)  // if ready for reading/receiving
                        {
                            // receive request
                            if(!receive_request(arrived_socket_fd))
                            {
                                logger::record("Error: can't receive request from socket fd: " + std::to_string(arrived_socket_fd));
                                continue;
                            }
                            logger::record("Successfully: receive response of socket fd: " + std::to_string(arrived_socket_fd));
                            logger::record(impl_->request->get_raw_request());
                            
                            // parse and generate request
                            if(!parse_request())
                            {
                                logger::record("Error: cant's parse request from socket fd: " + std::to_string(arrived_socket_fd));
                                continue;
                            }
                            logger::record("Successfully: parse response of socket fd: " + std::to_string(arrived_socket_fd));

                            ev.data.fd = arrived_socket_fd;
                            // Edge-triggered 
                            ev.events = EPOLLOUT | EPOLLET;
                            epoll_ctl(epfd, EPOLL_CTL_MOD, arrived_socket_fd, &ev);
                            continue;
                        }
                        else if (arrived_socket_event & EPOLLOUT) // if ready for writing/sending
                        {
                            // generate response
                            if(!generate_response())
                            {
                                logger::record("Error: can't generate response for socket fd: " + arrived_socket_fd);
                                continue;
                            }
                            logger::record("Successfully: generate response for socket fd: " + std::to_string(arrived_socket_fd));
                            logger::record(impl_->response->get_response_message());

                            // send response
                            if(!send_response(arrived_socket_fd))
                            {
                                logger::record("Error: can't send response to socket fd: " + std::to_string(arrived_socket_fd));
                                continue;
                            }
                            logger::record("Successfully: send response to socket fd: " + std::to_string(arrived_socket_fd));

                            // after send response, we remove client socket from event interest list.
                            epoll_ctl(epfd, EPOLL_CTL_DEL, arrived_socket_fd, &ev);
                            continue;
                        }
                    }
                }            
            }
            break;
        }
    }
}

bool Server::send_response(const int clietn_socket_fd)
{
    int sendResult = send(
        clietn_socket_fd, 
        impl_->response->get_response_message().c_str(), 
        impl_->response->get_response_length(), 
        0
    );

    if(sendResult == -1)
    {
        return false;
    }
    return true;
}

bool Server::receive_request(const int client_socket_fd)
{
    int receive_result = recv(client_socket_fd, impl_->received_buffer, impl_->buffer_length, 0);
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
}

bool Server::parse_request()
{
    // If request contains WebSocket header field, try upgrade to WebSocket protocol
    if(impl_->request->get_raw_request().find("WebSocket") != std::string::npos)
    {
        logger::record("Accept WebSocket request");
        websocket websocket(impl_->request, impl_->response);
        websocket.parse_websocket_request();
        websocket.generate_websocket_request();
        return true;
    }

    if(!impl_->request->parse_raw_request())
    {
        // 400 Bad Reques
        impl_->response->handle_status_code(400);
        return true;
    }

    // read requested resources
    if(!impl_->response->set_content(impl_->request->get_request_uri()))
    {
        // 404 Not Found
        impl_->response->handle_status_code(404);
        return true;
    }
    impl_->response->handle_status_code(200);

    return true;
}

bool Server::generate_response()
{
    // If successfully parse request, try to add requested resouces to response
    if(impl_->response->get_status_code() == 200)
    {
        if(!impl_->response->set_content(impl_->request->get_request_uri()))
        {
            // if we cann't set content, response 404 not found
            impl_->response->set_status(404);
            impl_->response->handle_status_code(404);
        }
    }

    return impl_->response->generate_response();
}

std::string Server::get_raw_request()
{
    return impl_->request->get_raw_request();
}

std::string Server::get_raw_response()
{
    return impl_->response->get_response_message();
}

int Server::get_client_fd()
{
    return impl_->client_socket;
}

int Server::get_server_fd()
{
    return impl_->server_socket;
}

void Server::set_raw_request(const std::string& raw_request)
{
    impl_->request->set_raw_request(raw_request);
}