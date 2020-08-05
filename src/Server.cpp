#include <sys/epoll.h>

#include "Server.hpp"
#include "websocket.hpp"
#include "logger.hpp"
#include "status_handler.hpp"

namespace
{
    /**
     * If request is a websocket opening handshake request.
     * This function is experimental. I'm not sure this is an appropriate way.
     * 
     * @param[in] request
     *      Request object that has been parsed successfully.
     * @return
     *      true if it's websocket opening handshake;
     *      false if it's not.
     */
    bool is_websocket_opening_handshake(std::shared_ptr< Message::Request >& request)
    {
        if(request->get_header("Upgrade") == "websocket")
        {
            return true;
        }
        return false;
    }
}

struct Server::Impl
{
    int client_socket;
    int server_socket;

    // Receive buffer
    static const int receive_buffer_length = 1024;
    char receive_buffer[receive_buffer_length] = { 0 };

    // Send buffer
    static const size_t SEND_BUFFER_LENGTH = 1024;
    std::string send_buffer = "";

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
                    int triggered_socket_fd = epoll_result[i].data.fd;
                    uint32_t arrived_socket_event = epoll_result[i].events;
                    
                    // If new client socket comes into the listening queue of server listening socket.
                    if(triggered_socket_fd == impl_->server_socket && (arrived_socket_event & EPOLLIN))
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
                    else if (triggered_socket_fd != impl_->server_socket) // If fds(exclude listen socket) triggered events
                    {
                        if(arrived_socket_event & EPOLLIN)  // if ready for reading/receiving
                        {
                            // receive request
                            if(!receive_request(triggered_socket_fd))
                            {
                                logger::record("Error: can't receive request from socket fd: " + std::to_string(triggered_socket_fd));
                                continue;
                            }
                            logger::record(impl_->request->get_raw_request());
                            logger::record("Successfully: receive request of socket fd: " + std::to_string(triggered_socket_fd));
                            
                            // parse and generate request
                            if(!request_core_handler())
                            {
                                logger::record("Error: cant's handle request from socket fd: " + std::to_string(triggered_socket_fd));
                                continue;
                            }
                            logger::record("Successfully: handle request of socket fd: " + std::to_string(triggered_socket_fd));

                            ev.data.fd = triggered_socket_fd;
                            ev.events = EPOLLOUT | EPOLLET;
                            epoll_ctl(epfd, EPOLL_CTL_MOD, triggered_socket_fd, &ev);
                            continue;
                        }
                        else if (arrived_socket_event & EPOLLOUT) // if ready for writing/sending
                        {
                            // generate response
                            if(!generate_response())
                            {
                                logger::record("Error: can't generate response for socket fd: " + triggered_socket_fd);
                                continue;
                            }
                            logger::record("Successfully: generate response for socket fd: " + std::to_string(triggered_socket_fd));
                            
                            // send response
                            if(!send_response(triggered_socket_fd, impl_->send_buffer))
                            {
                                logger::record("Error: can't send response to socket fd: " + std::to_string(triggered_socket_fd));
                                continue;
                            }
                            logger::record("Successfully: send response to socket fd: " + std::to_string(triggered_socket_fd));
                            logger::record(impl_->response->get_response_message());
                            
                            ev.data.fd = triggered_socket_fd;
                            ev.events = EPOLLIN | EPOLLET;
                            epoll_ctl(epfd, EPOLL_CTL_MOD, triggered_socket_fd, &ev);
                            continue;
                        }
                    }
                }            
            }
            break;
        }
    }
}

bool Server::send_response(const int clietn_socket_fd, const std::string& buffer)
{
    if(buffer.size() == 0)
    {
        return false;    
    }

    int send_result = send(
        clietn_socket_fd, 
        buffer.c_str(),
        buffer.size(),
        0
    );

    if(send_result == -1)
    {
        return false;
    }

    return true;
}

bool Server::receive_request(const int client_socket_fd)
{
    int receive_result = recv(client_socket_fd, impl_->receive_buffer, impl_->receive_buffer_length, 0);
    if(receive_result == 0 || receive_result == -1)
    {
        impl_->request->set_raw_request("");
        return false;
    }
    
    if(!impl_->request->set_raw_request(impl_->receive_buffer))
    {
        return false;
    }

    memset(impl_->receive_buffer, 0, impl_->receive_buffer_length);

    return true;
}

bool Server::parse_request()
{
    if(!impl_->request->parse_raw_request())
    {
        // 400 Bad Request
        status_handler::handle_status_code(impl_->response, 400);
        return false;
    }
    return true;
}

bool Server::generate_response()
{
    if(!impl_->response->generate_response())
    {
        return false;
    }
    impl_->send_buffer = impl_->response->get_response_message().c_str();
    return true;
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

bool Server::request_core_handler()
{
    // Parse request
    if(!parse_request())
    {
        // 400 Bad Request
        status_handler::handle_status_code(impl_->response, 400);
        return false;
    }

    // Is websocket opening handshake?
    if(
        impl_->request->has_header("Upgrade")
        && impl_->request->get_header("Upgrade") == "websocket"
    )
    {
        websocket websocket(impl_->request, impl_->response);
        if(!websocket.parse_websocket_request())
        {
            return false;
        }
        impl_->response->add_header("Sec-WebSocket-Accept", websocket.generate_sec_websocket_key());
        status_handler::handle_status_code(impl_->response, 101);
        return true;
    }

    // Resources handler
    if(
        !impl_->response->set_content(impl_->request->get_request_uri())
    )
    {
        // 404 Not Found
        status_handler::handle_status_code(impl_->response, 404);
        return false;
    }

    // Status handler
    status_handler::handle_status_code(impl_->response, 200);

    return true; 
}