#include "Server_Socket.hpp"

#include <stdexcept>
#include <iostream>

Server_Socket::Server_Socket()
{
    set_server_status( Server_Status::CLOSED );
}

void Server_Socket::listen_at( const std::string ip, const int port, const long timeout_microseconds ) 
{
    // If user sets timeout
    if(timeout_microseconds != -1)
    {
        listen_time_out_duration.tv_usec = timeout_microseconds;
    }

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

    set_server_status( Server_Status::LISTENING );

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    add_socket_to_read_fds( server_listening_socket );
    add_socket_to_write_fds( server_listening_socket );
    
    /**
     * The parameter read_fds identifies the sockets that are to be checked for readability. 
     * If the socket is currently in the listen state, it will be marked as readable if 
     * an incoming connection request has been received such that an accept is guaranteed 
     * to complete without blocking.
     */
    int select_result = select(
        0,
        &read_fds,
        &write_fds,
        nullptr,
        &listen_time_out_duration
    );
    
    if( select_result > 0 )
    {
        std::cout << "Server: I accepts one connection" << std::endl;

        SOCKET accept_result = accept(
            server_listening_socket,
            nullptr,
            nullptr
        );

        if( accept_result == INVALID_SOCKET )
        {
            WSACleanup();
            throw std::runtime_error("Can not accept socket");
        }     
    }
    else if( select_result == 0 )   // timeout
    {
        set_server_status( Server_Status::CLOSED );
        closesocket(server_listening_socket);
        WSACleanup();
        return;
    }
    else if( select_result == SOCKET_ERROR )    // error occurs
    {
    #ifdef _WIN32
        set_server_status( Server_Status::CLOSED );
        wchar_t* error_info = nullptr;
        FormatMessageW(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
            nullptr, 
            WSAGetLastError(),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPWSTR)&error_info, 
            0, 
            nullptr
        );
        closesocket(server_listening_socket);
        WSACleanup();
        std::wstring error_string(error_info);
        throw std::runtime_error( std::string( error_string.begin(), error_string.end() ) );
    #endif
    }

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

Server_Status Server_Socket::get_current_server_status()
{
    return server_status;
}

void Server_Socket::add_socket_to_read_fds( SOCKET socket )
{
    FD_SET(socket, &read_fds);
}

void Server_Socket::add_socket_to_write_fds( SOCKET socket )
{
    FD_SET(socket, &write_fds);
}

void Server_Socket::remove_socket_from_read_fds( SOCKET socket )
{
    
}   

void Server_Socket::remove_socket_from_write_fds( SOCKET socket )
{

}

void Server_Socket::set_server_status( Server_Status new_status )
{
    std::unique_lock< std::mutex > lock( server_status_mutex );
    server_status = new_status;
}

void Server_Socket::stop_listening()
{
    closesocket(server_listening_socket);
    WSACleanup();
}