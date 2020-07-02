#include <gtest/gtest.h>
#include "Uri.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Client.hpp"

#include <thread>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

bool epoll_reactor(Server& server)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("sock");
        return false;
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_port = htons(2333);
    local.sin_addr.s_addr = inet_addr("localhost");

    if (bind(sock, (struct sockaddr*)&local, sizeof(local))<0)
    {
        perror("bind");
        exit(3);
    }

    if (listen(sock, 10) < 0)
    {
        perror("listen");
        exit(4);
    }

    int epfd = epoll_create(1024);
    if(epfd == -1)
    {
        perror("Error epoll create");
        return false;
    }

    int listen_sock = server.get_server_fd();
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
        return false;

    int nums = -1;
    struct epoll_event read_ready[1024];
    while (true)
    {
        switch((nums = epoll_wait(epfd, read_ready, 10, -1)))
        {
            case 0:
                perror("time out");
                break;
            case -1:
                perror("epoll wait");
                break;
            default:
                int i = 0;
                for(; i<nums; ++i)
                {
                    int sock = read_ready[i].data.fd;
                    if(sock == listen_sock && (read_ready[i].events & EPOLLIN))
                    {
                        // struct addrinfo hints;
                        // hints.ai_family = AF_INET;
                        // hints.ai_socktype = SOCK_STREAM;
                        // hints.ai_protocol = 0;
                        // hints.ai_flags = 0;

                        int new_socket = accept(listen_sock, NULL, NULL);
                        if(new_socket == -1)
                        {
                            perror("accept error");
                            continue;
                        }
                        else
                        {
                            ev.events = EPOLLIN;
                            ev.data.fd = new_socket;
                            epoll_ctl(epfd, EPOLL_CTL_ADD, new_socket, &ev);
                        }

                    }
                    else if (sock != listen_sock)
                    {
                        if(read_ready[i].events & EPOLLIN)
                        {
                            // TAT
                            char *buf[10240];
                            ssize_t s = read(sock, buf, sizeof(buf) - 1 );
                            if(s>0)
                            {
                                ev.events = EPOLLOUT;
                                ev.data.fd = sock;
                                epoll_ctl(epfd, EPOLL_CTL_MOD, sock, &ev);
                            }
                            else if (s == 0)
                            {
                                close(sock);
                                epoll_ctl(epfd, EPOLL_CTL_DEL, sock, NULL);
                            }
                            else
                            {
                                perror("read");
                                close(sock);
                                epoll_ctl(epfd, EPOLL_CTL_DEL, sock, NULL);
                            }
                        }
                        else if ( sock != listen_sock)
                        {
                            
                            std::string msg = "HTTP/1.0 200 OK\r\n\r\n <html><h1> Hello Bitate! </h1></html>\r\n";
                            write(sock, msg.c_str(), msg.size());
                            close(sock);
                            epoll_ctl(epfd, EPOLL_CTL_DEL, sock, NULL);
                        
                        }
                        else
                        {}
                    }
                }
                break;
        }
    }
    
    return 0;
}

TEST(server_tests, listen_at_test)
{
    Server server;
    ASSERT_TRUE(server.listen_at("localhost", 2333));
    //ASSERT_EQ(server.get_server_fd(), 3);
} 

TEST(server_tests, generate_response)
{
    Server server;
    ASSERT_TRUE(server.generate_response());
    ASSERT_EQ(server.get_raw_request(), "");
}



TEST(server_tests, todos)
{
    // Client client;

    // ASSERT_TRUE(server.listen_at("localhost", 2333));
    // ASSERT_TRUE(server.receive_request());
    // ASSERT_TRUE(server.parseRequest());
    // ASSERT_TRUE(server.generate_response());
    // ASSERT_TRUE(server.send_response());    
    // epoll_reactor(server);
    
    // ASSERT_TRUE(client.parse_uri("http://localhost:8088/"));
    // ASSERT_TRUE(client.connect_to());
    // ASSERT_TRUE(client.generate_request());
    // ASSERT_TRUE(client.send_request());

    // ASSERT_EQ(client.get_generated_request(), server.get_raw_request());
}