#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <iostream>

TEST(thread_pool_tests, simple_test)
{
    for(int i = 0; i< 100; ++i)
    {
        Thread_Pool thread_pool;
    
        std::string message = "This is task: ";
        
        auto logger = [i](const std::string& message){
            std::cout << message << i << std::endl;
        };

        thread_pool.post_task( [&]{ logger(message); } );
        
        thread_pool.shutdown_thread_pool();
    }
    
}