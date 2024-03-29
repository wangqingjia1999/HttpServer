#include "Master.hpp"
#include <exception>
#include <iostream>

int main()
{
	try
	{
		Master::listen_at("0.0.0.0", 40000);
	}
	catch (const std::exception& e)
	{
		std::cout << "Exception thrown: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}