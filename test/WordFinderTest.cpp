#include "Server.hpp"
#include <exception>
#include <iostream>

int main()
{
    try
    {
        Server server;
        server.listen_at("0.0.0.0", 80);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS
}