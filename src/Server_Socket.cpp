#include "Server_Socket.hpp"

Server_Socket::Server_Socket()
{
}

void Server_Socket::listen_at( const std::string ip, const int port) 
{
#ifdef _WIN32
    WSADATA wsa_data = {0};
    // Request WinSock version 2.2 
    auto result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if(result != NO_ERROR)
    {
        print_socket_error();
    }

    server_listening_socket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if(server_listening_socket == INVALID_SOCKET)
    {
        print_socket_error();
    }

    sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.S_un.S_addr = inet_addr(ip.c_str());
    socket_address.sin_port = htons(port);

    int bind_result = bind(
        server_listening_socket, 
        (sockaddr*)&socket_address, 
        sizeof(socket_address)
    );

    if( bind_result == INVALID_SOCKET )
    {
        print_socket_error();
    }

    listen_result = listen( server_listening_socket, 1024 );
  
#elif __linux__
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1)
    {
        return false;
    }

    struct sockaddr_in ipv4_address;
    memset(&ipv4_address, 0 , sizeof(ipv4_address));
    ipv4_address.sin_family = AF_INET;
    ipv4_address.sin_addr.s_addr = inet_addr(host.c_str());
    ipv4_address.sin_port = htons(port);
    int ipv4_address_length = sizeof(ipv4_address);

    int bind_result = bind(server_socket, (struct sockaddr*)&ipv4_address, ipv4_address_length);
    if(bind_result == -1)
    {
        return false;
    }

    int listen_result = listen(server_socket, 4096);
    if(listen_result == -1)
    {
        return false;
    }    
    
    add_epoll_wait_read_event(server_socket);

    // server core loop
    for (;;)
    {
        int triggered_fd_numbers = epoll_wait(epfd, epoll_events_result, 1024, -1);
        switch (triggered_fd_numbers)
        {
        case 0:
            thread_pool->post_task( []{ Logger::record_error("epoll wait, because wait time out"); } );
            break;
        case -1:
            thread_pool->post_task( []{ Logger::record_error("epoll wait"); } );
            break;
        default:
            {
                int i;
                for(i = 0; i < triggered_fd_numbers; ++i)
                {
                    int triggered_socket_fd = epoll_events_result[i].data.fd;
                    uint32_t arrived_socket_event = epoll_events_result[i].events;
                    
                    // The listening socket is ready; this means a new peer/client is connecting.
                    if( (triggered_socket_fd == server_socket) && (arrived_socket_event & EPOLLIN))
                    {
                        struct sockaddr_in client_socket_address;
                        socklen_t client_socket_length = sizeof(client_socket_address);

                        client_socket = accept(
                            server_socket, 
                            (struct sockaddr*)&client_socket_address, 
                            &client_socket_length 
                        );

                        if(client_socket < 0)
                        {
                            thread_pool->post_task( []{ Logger::record_error("accept client socket"); } );
                            continue;
                        }
                        
                        // add new client socket into epoll wait-read list.
                        add_epoll_wait_read_event(client_socket);
                        continue;
                    }
                    else if (triggered_socket_fd != server_socket)
                    {
                        if(arrived_socket_event & EPOLLIN)  // if ready for reading/receiving
                        {
                            // receive request
                            thread_pool->post_task(
                                [this, triggered_socket_fd]{
                                    this->receive_request(triggered_socket_fd);
                                }
                            );
                            
                            // parse and generate request
                            thread_pool->post_task(
                                [this]{
                                    this->request_core_handler();
                                }
                            );

                            add_epoll_wait_write_event(triggered_socket_fd);
                            continue;
                        }
                        else if (arrived_socket_event & EPOLLOUT) // if ready for writing/sending
                        {
                            thread_pool->post_task(
                                [this, triggered_socket_fd]{
                                    this->send_response(triggered_socket_fd, response->get_response_message());
                                }
                            );

                            add_epoll_wait_read_event(triggered_socket_fd);
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
    return;
}


void Server_Socket::write_to(const std::string& data)
{
    int send_result = send(
        client_socket,
        data.c_str(),
        data.size(),
        0
    );

    if(send_result == SOCKET_ERROR)
        print_socket_error();
}

std::string Server_Socket::read_from()
{
    const int data_buffer_size = 1024;
    char data_buffer[data_buffer_size];

    // local variable that stores the data in the form of std::string.
    std::string data_string;

    int receive_result = 0;
    do{
        receive_result = recv(
            client_socket,
            data_buffer,
            data_buffer_size,
            0
        );

        if(receive_result == SOCKET_ERROR)
            print_socket_error();

        data_string += data_buffer;
        
        receive_result -= data_buffer_size;

    } while( receive_result > 0 );
    return data_string;
}

void Server_Socket::print_socket_error()
{
    wchar_t *s = NULL;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                NULL, WSAGetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPWSTR)&s, 0, NULL);
    fprintf(stderr, "%S\n", s);
    LocalFree(s);
    WSACleanup();
}

bool Server_Socket::has_client_connection()
{
    if(listen_result == SOCKET_ERROR)
    {
        print_socket_error();
    }
    else
    {
        std::cout << "I'm listening right now" << std::endl;
    }

    client_socket = accept(
        server_listening_socket,
        nullptr,
        nullptr
    );

    if( client_socket == INVALID_SOCKET )
    {
        print_socket_error();
    }
    else
    {
        std::cout << "I accept a socket" << std::endl;
        return true;
    }
    
    return false;
}