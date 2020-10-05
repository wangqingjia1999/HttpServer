#include "Thread_Pool.hpp"

#include <gtest/gtest.h>

/**
 * @brief  Ensure the ordering of execution of functions/works.
 *         First in, first get executed.
 */
TEST(thread_pool_tests, work_ordering_test)
{
    auto thread_pool = std::make_shared< Thread_Pool >();

    for(int i = 0; i< 100; ++i)
    {   
        auto get_number = [](const int number){
            return number;
        };

        thread_pool->post_task( 
            [get_number, i]{
                EXPECT_EQ( get_number(i), i );
            }
        );
    }
    thread_pool->shutdown_thread_pool();
}