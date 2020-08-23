#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <iostream>

bool try_it()
{
    return true;
}
TEST(thread_pool_tests, simple_test)
{
    Thread_Pool thread_pool;
    
    std::string m = "Demo";
    auto logger = [](const std::string& message){
        std::cout << message << std::endl;
    };

    thread_pool.post_task( [&]{ logger(m); } );
}