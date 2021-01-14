#include "../../include/Client_Socket.hpp"

#include <string>
#include <iostream>

int main()
{
    Client_Socket client_socket;
    client_socket.connect_to("127.0.0.1", 18135);

    for(;;)
    {
        printf("Type whatever you want here:\n");

        std::string line;
        std::string echo_string;
        while(std::getline(std::cin, line) && !line.empty())
        {
            echo_string += line;
        }

        client_socket.fill_send_buffer(echo_string);
        client_socket.write_to();
        client_socket.read_from();        
    }
}