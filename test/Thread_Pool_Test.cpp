#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(thread_pool_tests, simple_test)
{
    Thread_Pool thread_pool;
    
    std::string message = "This is a simple test";
    
    auto logger = [](const std::string& message){
        std::cout << message << std::endl;
    };

    thread_pool.post_task( [&]{ logger(message); } );
    thread_pool.shutdown_thread_pool();
}