#include "Server.hpp"
#include "Mysql_Handler.hpp"
#include "WebSocket.hpp"
#include "Logger.hpp"
#include "Status_Handler.hpp"
#include "Thread_Pool.hpp"

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

    // Epoll file descriptor
    int epfd = epoll_create(1024);
    
    // Epoll events
    struct epoll_event epoll_events;

    // Epoll events result list
    struct epoll_event epoll_events_result[1024];
    
    // Response object for generating response
    std::shared_ptr< Message::Response> response = std::make_shared< Message::Response >();

    // Server side request object that is responsible for parseing coming request.
    std::shared_ptr< Message::Request> request = std::make_shared< Message::Request >();

    /**
     * Handler for mysql which is responsible for managing the mysql.
     */
    Mysql_Handler mysql;
    
    /**
     * Data obtained from POST request, I just store those name:value pairs into a map :).
     */
    std::map< std::string, std::string > post_data_map;

    /**
     * Thread pool to avoid the unnecessary creation and deletion of multiple threads.
     */
    std::shared_ptr< Thread_Pool > thread_pool = std::make_shared< Thread_Pool >();

    void add_epoll_wait_read_event(int target_fd)
    {
        epoll_events.events = EPOLLIN | EPOLLET;
        epoll_events.data.fd = target_fd;
        epoll_ctl(epfd, EPOLL_CTL_ADD, target_fd, &epoll_events);
    }

    void add_epoll_wait_write_event(int target_fd)
    {
        epoll_events.events = EPOLLOUT | EPOLLET;
        epoll_events.data.fd = target_fd;
        epoll_ctl(epfd, EPOLL_CTL_DEL, target_fd, &epoll_events);
    }

    void delete_from_epoll_events_list(int target_fd)
    {
        epoll_events.data.fd = target_fd;
        epoll_ctl(epfd, EPOLL_CTL_DEL, target_fd, &epoll_events);
    }
};

Server::~Server()
{
}
Server::Server() : impl_(new Impl)
{
}

bool Server::listen_at(const std::string& host, const int port)
{
    impl_->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(impl_->server_socket == -1)
    {
        return false;
    }

    struct sockaddr_in ipv4_address;
    memset(&ipv4_address, 0 , sizeof(ipv4_address));
    ipv4_address.sin_family = AF_INET;
    ipv4_address.sin_addr.s_addr = inet_addr(host.c_str());
    ipv4_address.sin_port = htons(port);
    int ipv4_address_length = sizeof(ipv4_address);

    int bind_result = bind(impl_->server_socket, (struct sockaddr*)&ipv4_address, ipv4_address_length);
    if(bind_result == -1)
    {
        return false;
    }

    int listen_result = listen(impl_->server_socket, 4096);
    if(listen_result == -1)
    {
        return false;
    }    
    
    impl_->add_epoll_wait_read_event(impl_->server_socket);

    // server core loop
    for (;;)
    {
        int triggered_fd_numbers = epoll_wait(impl_->epfd, impl_->epoll_events_result, 1024, -1);
        switch (triggered_fd_numbers)
        {
        case 0:
            impl_->thread_pool->post_task( []{ Logger::record_error("epoll wait, because wait time out"); } );
            break;
        case -1:
            impl_->thread_pool->post_task( []{ Logger::record_error("epoll wait"); } );
            break;
        default:
            {
                int i;
                for(i = 0; i < triggered_fd_numbers; ++i)
                {
                    int triggered_socket_fd = impl_->epoll_events_result[i].data.fd;
                    uint32_t arrived_socket_event = impl_->epoll_events_result[i].events;
                    
                    // The listening socket is ready; this means a new peer/client is connecting.
                    if( (triggered_socket_fd == impl_->server_socket) && (arrived_socket_event & EPOLLIN))
                    {
                        struct sockaddr_in client_socket_address;
                        socklen_t client_socket_length = sizeof(client_socket_address);

                        impl_->client_socket = accept(
                            impl_->server_socket, 
                            (struct sockaddr*)&client_socket_address, 
                            &client_socket_length 
                        );

                        if(impl_->client_socket < 0)
                        {
                            impl_->thread_pool->post_task( []{ Logger::record_error("accept client socket"); } );
                            continue;
                        }
                        
                        // add new client socket into epoll wait-read list.
                        impl_->add_epoll_wait_read_event(impl_->client_socket);
                        continue;
                    }
                    else if (triggered_socket_fd != impl_->server_socket)
                    {
                        if(arrived_socket_event & EPOLLIN)  // if ready for reading/receiving
                        {
                            // receive request
                            impl_->thread_pool->post_task(
                                [this, triggered_socket_fd]{
                                    this->receive_request(triggered_socket_fd);
                                }
                            );
                            
                            // parse and generate request
                            impl_->thread_pool->post_task(
                                [this]{
                                    this->request_core_handler();
                                }
                            );

                            impl_->add_epoll_wait_write_event(triggered_socket_fd);
                            continue;
                        }
                        else if (arrived_socket_event & EPOLLOUT) // if ready for writing/sending
                        {
                            impl_->thread_pool->post_task(
                                [this, triggered_socket_fd]{
                                    this->send_response(triggered_socket_fd, impl_->response->get_response_message());
                                }
                            );

                            impl_->add_epoll_wait_read_event(triggered_socket_fd);
                            continue;
                        }
                    }
                }            
            }
            break;
        }
    }
}

void Server::send_response(const int clietn_socket_fd, const std::string& buffer)
{
    if(buffer.size() == 0)
    {
        return;    
    }

    int send_result = send(
        clietn_socket_fd, 
        buffer.c_str(),
        buffer.size(),
        0
    );

    if(send_result == -1)
    {
        impl_->thread_pool->post_task( []{ Logger::record_error("send response"); } );
        return;
    }
    return;
}

void Server::receive_request(const int client_socket_fd)
{
    int receive_result = recv(client_socket_fd, impl_->receive_buffer, impl_->receive_buffer_length, 0);
    if(receive_result == 0 || receive_result == -1)
    {
        impl_->request->set_raw_request("");
        return;
    }

    if(!impl_->request->set_raw_request(impl_->receive_buffer))
    {
        impl_->thread_pool->post_task( []{ Logger::record_error("set raw requst"); } );
        return;
    }

    memset(impl_->receive_buffer, 0, impl_->receive_buffer_length);

    return;
}

bool Server::parse_request()
{
    return impl_->request->parse_raw_request();
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
        
        try
        {
            impl_->mysql.add_user(
            impl_->post_data_map.find("Name")->second,
            std::stoi(impl_->post_data_map.find("Age")->second),
            impl_->post_data_map.find("Email")->second,
            impl_->post_data_map.find("Password")->second
            );
        }
        catch(const std::exception& exception_message)
        {
            impl_->thread_pool->post_task(
                [exception_message]{
                    Logger::record(exception_message.what());
                }
            );
        }
    }
    return true;
}

void Server::request_core_handler()
{
    // Parse request
    if(!parse_request())
    {
        // 400 Bad Request
        impl_->thread_pool->post_task( []{ Logger::record_error("parse request"); } );
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
            impl_->thread_pool->post_task( []{ Logger::record_error("parse POST request"); } );
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
            impl_->thread_pool->post_task( []{ Logger::record_error("parse WebSocket request"); } );
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
        impl_->thread_pool->post_task( []{ Logger::record_error("handle requested resources"); } );
        return;
    }

    // Status handler
    Status_Handler::handle_status_code(impl_->response, 200);
}