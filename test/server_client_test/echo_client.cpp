#include "../../include/Client_Socket.hpp"

#include <string>
#include <iostream>

int main()
{
    Client_Socket client_socket;
    client_socket.connect_to("127.0.0.1", 18135);
    
    for(;;)
    {
        std::cout << "Type whatever you like" << std::endl;
        
        std::string line;
        std::string echo_string;
        while(std::getline(std::cin, line) && !line.empty())
        {
            echo_string += line;
        }

        std::cout << echo_string;

        std::vector<uint8_t> local_send_buffer;
        for(const auto& echo_char : echo_string)
            local_send_buffer.push_back( (uint8_t)echo_char );

        client_socket.fill_send_buffer(echo_string);

        client_socket.write_to(
            client_socket.get_client_fd(),
            local_send_buffer,
            local_send_buffer.size()
        );

        std::vector<uint8_t>* local_receive_buffer = client_socket.read_from(client_socket.get_client_fd());
        std::string receive_buffer_string;
        for(const auto& byte : *local_receive_buffer)
            receive_buffer_string += (char)byte;
        
        std::cout << receive_buffer_string;
    }
}