#include "Master.hpp"
#include <exception>
#include <iostream>

int main()
{
    try
    {
        Master master("0.0.0.0", 80);
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception thrown: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}