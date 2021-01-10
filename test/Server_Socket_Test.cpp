#include "Server_Socket.hpp"
#include "Client_Socket.hpp"

#include <gtest/gtest.h>
#include <unistd.h>

TEST(server_socket_tests, todo)
{
    pid_t child_pid;
    switch(child_pid = fork())
    {
        case -1:
        {
            fprintf(stderr, "Cannot create child process.\n");
        }

        case 0: // child process
        {
            Client_Socket client_socket;
            client_socket.connect_to("127.0.0.1", 12345);
        }
        
        case 1: // parent process 
        {
            Server_Socket server_socket;
            server_socket.listen_at("127.0.0.1", 12345);
        }
    }
}

