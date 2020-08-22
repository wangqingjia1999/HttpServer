#include "Server.hpp"
#include "Mysql_Handler.hpp"
#include "WebSocket.hpp"
#include "Logger.hpp"
#include "Status_Handler.hpp"
#include "Thread_Pool.hpp"

#include <sys/epoll.h>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>


namespace
{
    /**
     * @brief  Whether the request is a WebSocket openning handshake request.
     * @param  request  Request object that has been parsed successfully.
     * @return  True if it is WebSocket openning handshake request.
     * 
     * Note: this is in experimental, I'm not ensure its satety :)
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

    Mysql_Handler mysql;
    
    std::map< std::string, std::string > post_data_map;

    std::shared_ptr< Thread_Pool > thread_pool = std::make_shared< Thread_Pool >();
};

Server::~Server()
{
}
Server::Server() : impl_(new Impl)
{
}

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
        int triggered_fd_numbers = epoll_wait(epfd, epoll_result, 1024, -1);
        switch (triggered_fd_numbers)
        {
        case 0:
            Logger::record("Wait: time out");
            
            break;
        case -1:
            Logger::record("Wait: error occurs");
            break;
        default:
            {
                int i;
                for(i=0; i < triggered_fd_numbers; ++i)
                {
                    int triggered_socket_fd = epoll_result[i].data.fd;
                    uint32_t arrived_socket_event = epoll_result[i].events;
                    
                    // The listening socket is ready; this means a new peer/client is connecting.
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
                            Logger::record("Failed to accept client socket");
                            continue;
                        }
                        
                        std::string ip_string = inet_ntoa(client_socket_address.sin_addr);
                        std::string port_string = std::to_string((client_socket_address.sin_port));
                        Logger::record("Accept connection from: " + ip_string + ":" + port_string);
                    
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
                                Logger::record("Error: can't receive request from socket fd: " + std::to_string(triggered_socket_fd));
                            }
                            else
                            {
                                Logger::record("Successfully: receive request of socket fd: " + std::to_string(triggered_socket_fd));
                            }
                            
                            // parse and generate request
                            impl_->thread_pool->add_work([this]{ this->request_core_handler(); });

                            ev.data.fd = triggered_socket_fd;
                            ev.events = EPOLLOUT | EPOLLET;
                            epoll_ctl(epfd, EPOLL_CTL_MOD, triggered_socket_fd, &ev);
                            continue;
                        }
                        else if (arrived_socket_event & EPOLLOUT) // if ready for writing/sending
                        {
                            impl_->thread_pool->add_work( [this]{ this->generate_response(); });
                            
                            if(!send_response(triggered_socket_fd, impl_->send_buffer))
                            {
                                Logger::record("Error: can not send response");
                                Logger::record("The generated response message is printed below: ");
                                Logger::record("------------------------------------------");
                                Logger::record(impl_->response->get_response_message());
                                Logger::record("------------------------------------------");
                            }
                            else
                            {
                                Logger::record("Successfully: send response for socket: " + std::to_string(triggered_socket_fd));
                            }
                            
                            ev.data.fd = triggered_socket_fd;
                            ev.events = EPOLLIN | EPOLLET;
                            epoll_ctl(epfd, EPOLL_CTL_DEL, triggered_socket_fd, &ev);
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
    return impl_->request->parse_raw_request();
}

void Server::generate_response()
{
    impl_->response->generate_response();
    impl_->send_buffer = impl_->response->get_response_message().c_str();
    Logger::record("Sending buffer is: \n");
    Logger::record(impl_->send_buffer);

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

bool Server::handle_post_request()
{
    /**
     * e.g. impl_->request->get_body() is "Name=Bitate&Age=21&Email=admin%40bitate.com&Password=qwerqwer"
     */
    URI::Percent_Encoding percent_encoding;
    /**
     * Append an additional '&', so that each name=value pair has a trailing '&'
     */
    std::string body_buffer = percent_encoding.decode(impl_->request->get_body()) + '&';

    while(!body_buffer.empty())
    {
        if(body_buffer.find_first_of('&') != std::string::npos)
        {
            if(body_buffer.find_first_of('=') != std::string::npos)
            {   
                std::string name = body_buffer.substr(0, body_buffer.find_first_of('='));
                
                std::string value = body_buffer.substr(
                    body_buffer.find_first_of('=') + 1, 
                    body_buffer.find_first_of('&') - body_buffer.find_first_of('=') - 1
                );

                impl_->post_data_map[std::move(name)] = std::move(value);

                body_buffer = body_buffer.substr(body_buffer.find_first_of('&')+1);
            }
        }
    }

    // add data to mysql
    if(!impl_->post_data_map.empty())
    {
        impl_->mysql.connect_to_mysql(3306, "bitate", "qwer");

        return impl_->mysql.add_user(
            impl_->post_data_map.find("Name")->second,
            std::stoi(impl_->post_data_map.find("Age")->second),
            impl_->post_data_map.find("Email")->second,
            impl_->post_data_map.find("Password")->second
        );
    }
    return true;
}

void Server::request_core_handler()
{
    // Parse request
    if(!parse_request())
    {
        // 400 Bad Request
        Logger::record("Can not parse request");
        Status_Handler::handle_status_code(impl_->response, 400);
        return;
    }

    // If it is POST request
    if(impl_->request->get_request_method() == "POST")
    {
        if(handle_post_request())
        {
            // Content created
            Status_Handler::handle_status_code(impl_->response, 200);
            impl_->response->add_header("Location", "/sign_up_done.html");
            impl_->response->set_content("/sign_up_done.html");
            impl_->response->add_header("Content-Type", "text/html");
            impl_->response->add_header("Content-Length", impl_->response->get_body_length());
            return;
        }
        else
        {
            // bad request
            Logger::record("Can not parse POST request");
            Status_Handler::handle_status_code(impl_->response, 400);
        }
    }

    // If it is websocket opening handshake
    if(
        impl_->request->has_header("Upgrade")
        && impl_->request->get_header("Upgrade") == "websocket"
    )
    {
        WebSocket websocket(impl_->request, impl_->response);
        if(!websocket.parse_websocket_request())
        {
            return;
        }
        impl_->response->add_header("Sec-WebSocket-Accept", websocket.generate_sec_websocket_key());
        Status_Handler::handle_status_code(impl_->response, 101);
        return;
    }

    // Resources handler
    if(
        impl_->request->get_request_method() == "GET"
        && !impl_->response->set_content(impl_->request->get_request_uri())
    )
    {
        // 404 Not Found
        Status_Handler::handle_status_code(impl_->response, 404);
        return;
    }

    // Status handler
    Status_Handler::handle_status_code(impl_->response, 200);

    Logger::record("OK: handle request :-) ");
}