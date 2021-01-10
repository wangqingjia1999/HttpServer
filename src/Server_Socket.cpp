#include "Server_Socket.hpp"

Server_Socket::Server_Socket()
    : listen_fd(-1),
      epfd(-1)
{
    epfd = epoll_create(1024);
    if(epfd < 0)
        fprintf(stderr, "Failed to create epoll file descriptor.\n");
}

Server_Socket::~Server_Socket()
{
    if(close(epfd))
        fprintf(stderr, "Failed to close epoll file descriptor.\n");
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

    listen_fd = listen( server_listening_socket, 1024 );
  
#elif __linux__
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
    {
        // log error
        return;
    }

    struct sockaddr_in ipv4_address;
    memset(&ipv4_address, 0 , sizeof(ipv4_address));
    ipv4_address.sin_family = AF_INET;
    ipv4_address.sin_addr.s_addr = inet_addr(ip.c_str());
    ipv4_address.sin_port = htons(port);
    int ipv4_address_length = sizeof(ipv4_address);

    int bind_result = bind(listen_fd, (struct sockaddr*)&ipv4_address, ipv4_address_length);
    if(bind_result == -1)
    {
        // log bind error
        return;
    }

    listen_fd = listen(listen_fd, 4096);
    if(listen_fd == -1)
    {
        // log bind error  
        perror("listen");
        return;
    }    
    
    printf("I'm listening");

    server_epoll_event.data.fd = listen_fd;
    server_epoll_event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &server_epoll_event);

    constexpr int TRIGGERED_EVENTS_MAX_SIZE = 1024;

    epoll_event triggered_events[TRIGGERED_EVENTS_MAX_SIZE];
    
    int number_of_triggered_events = epoll_wait(epfd, triggered_events, TRIGGERED_EVENTS_MAX_SIZE, -1);

    // server main loop
    for (;;)
    {
        switch(number_of_triggered_events)
        {
            case -1:
                perror("epoll");
                break;

            default:
            {
                int i = 0;
                for(; i < number_of_triggered_events; ++i)
                {
                    if(triggered_events[i].data.fd == listen_fd && (triggered_events[i].events & EPOLLIN))
                    {
                        sockaddr_in client_socket;
                        socklen_t client_socket_length = sizeof(client_socket);

                        int client_fd = accept(listen_fd, (sockaddr*)&client_socket, &client_socket_length);
                        if(client_fd < 0)
                        {
                            fprintf(stderr, "Accept error!");
                            continue;
                        }

                        printf("Accept client from : [%s] [%d]\n", inet_ntoa(client_socket.sin_addr), htons(client_socket.sin_port));
                        
                        server_epoll_event.events = EPOLLIN;
                        server_epoll_event.data.fd = client_fd;
                        epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &server_epoll_event);
                    }
                    else if(triggered_events[i].data.fd != listen_fd) // if the connection has been established
                    {
                        if(triggered_events[i].events & EPOLLIN)
                        {
                            ssize_t read_result = read(triggered_events[i].data.fd, receive_buffer, RECEIVE_BUFFER_SIZE - 1);
                            if(read_result > 0)
                            {
                                printf("Read from client : %s \n", receive_buffer);
                                server_epoll_event.events = EPOLLOUT;
                                server_epoll_event.data.fd = triggered_events[i].data.fd;
                                epoll_ctl(epfd, EPOLL_CTL_MOD, triggered_events->data.fd, &server_epoll_event);
                            }
                            else if(read_result == 0)
                            {
                                printf("Client quit\n");
                                close(triggered_events[i].data.fd);
                                epoll_ctl(epfd, EPOLL_CTL_DEL, triggered_events[i].data.fd, NULL);
                            }
                            else
                            {
                                perror("read");
                                close(triggered_events[i].data.fd);
                                epoll_ctl(epfd, EPOLL_CTL_DEL, triggered_events[i].data.fd, NULL);
                            }
                        }
                        else if(triggered_events[i].events & EPOLLOUT)
                        {
                            const char *msg = "HTTP/1.0  OK 200 \r\n\r\n<html><h1>WX    EPOLL  </h1></html>";
                            write(triggered_events[i].data.fd, send_buffer, SEND_BUFFER_SIZE);
                            close(triggered_events[i].data.fd);
                            epoll_ctl(epfd, EPOLL_CTL_DEL, triggered_events[i].data.fd, NULL);
                        }
                        else
                        {
                            printf("Undefined epoll behaviour.");
                        }
                    }
                }
            }
            break;
        }    
    }

#endif
    return;
}


size_t Server_Socket::write_to(const int peer_socket, const char* data_buffer, const int data_size)
{
    return send(peer_socket, &data_buffer, data_size, 0);
}

size_t Server_Socket::read_from(const int peer_socket, char* data_buffer, const int data_size)
{
    return recv(peer_socket, receive_buffer, RECEIVE_BUFFER_SIZE, 0);
}

void Server_Socket::print_socket_error()
{
#ifdef _WIN32
    wchar_t *s = NULL;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                NULL, WSAGetLastError(),
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPWSTR)&s, 0, NULL);
    fprintf(stderr, "%S\n", s);
    LocalFree(s);
    WSACleanup();
#elif __linux__

#endif

}
