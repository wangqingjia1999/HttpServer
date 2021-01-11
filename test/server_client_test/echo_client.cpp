#include "../../include/Client_Socket.hpp"

#include <string>

int main()
{
    Client_Socket client_socket;
    client_socket.connect_to("127.0.0.1", 18135);
    
    std::string message = "Hello server! I'm client!";
    client_socket.write_to(
        client_socket.get_client_fd(),
        message.c_str(),
        message.size()
    );
}