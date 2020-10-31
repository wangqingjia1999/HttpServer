#include "Server_Socket.hpp"

#include <stdexcept>
#include <iostream>

void Server_Socket::listen_at( const std::string ip, const int port, const long long millisecond ) 
{
    if(millisecond != -1)
        listening_timer.reset_start_time();

#ifdef _WIN32
    WSADATA wsa_data = {0};
    // Request WinSock version 2.2 
    auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if(result != NO_ERROR)
    {
        WSACleanup();
        throw std::runtime_error("Can not initialize WSADATA");
    }

    server_listening_socket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if(server_listening_socket == INVALID_SOCKET)
    {
        WSACleanup();
        throw std::runtime_error("Can not create a new socket");
    }

    sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.S_un.S_addr = inet_addr( ip.c_str() );
    socket_address.sin_port = htons(port);

    int bind_result = bind(
        server_listening_socket, 
        (sockaddr*)&socket_address, 
        sizeof(socket_address)
    );

    if(bind_result == INVALID_SOCKET)
    {
        WSACleanup();
        throw std::runtime_error("Can not bind a socket");
    }

    int listen_result = listen(server_listening_socket, 1024);

    int select_result = select(
        0,
        &read_fds,
        &write_fds,
        nullptr,
        &listen_time_out_duration
    );

    closesocket(server_listening_socket);
    WSACleanup();
    return;

#elif __linux__
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
    
    return;
#endif
}


void Server_Socket::write_to(const int peer_socket, const char* data_buffer, int data_length)
{
    int write_result = send(peer_socket, data_buffer, data_length, 0);

    if(write_result == SOCKET_ERROR)
    {
        throw std::runtime_error("Can not send to peer socket");
    }

    return;
}

void Server_Socket::read_from(const int peer_socket, char* data_buffer, int data_length)
{
    int read_result = recv(peer_socket, data_buffer, data_length, 0);

    if(read_result == SOCKET_ERROR)
    {
        throw std::runtime_error("Can not read from peer socket");
    }

    return;
}