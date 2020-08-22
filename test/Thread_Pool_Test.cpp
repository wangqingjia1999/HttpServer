#include "Thread_Pool.hpp"

#include <gtest/gtest.h>

TEST(thread_pool_tests, simple_test)
{
    Thread_Pool thread_pool;
    
    thread_pool.add_work([](){
        std::cout << "this is work 1" << std::endl;
    });
}