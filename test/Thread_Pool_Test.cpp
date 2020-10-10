#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <mutex>
#include <future>
#include <iostream>


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

/**
 * Suppose I have 1 million integers. I split all numbers into 10 groups/tasks.
 * Now, I'm going to use multithreading to fill these numbers into a vector with
 * the capacity of 1 million and the index of [1, 1 million].
 */
TEST(thread_pool_tests, fill_in_vector_test)
{
    auto thread_pool = std::make_shared< Thread_Pool >();

    constexpr int one_thousand = 1000;
    constexpr int one_million = one_thousand*one_thousand;

    std::vector<int> result_set(one_million);   

    std::mutex result_set_mutex;
    std::promise<bool> promise_obj;
    std::future<bool> future_obj = promise_obj.get_future();
    

    auto fill_in_function = [&](std::vector<int>& result, int begin, int end, std::promise< bool >& promise){
        while(begin < end)
        {
            {
                std::lock_guard< std::mutex > lock(result_set_mutex); 
                result[begin] = begin;
                ++begin;
            }
            if(begin == 999)
                    promise.set_value(true);
        }
        return;
    };

    /**
     * Split the taskes to 10 sub-taskes
     */
    for(int i = 0; i < 1000; i += 1000/10 )
    {
        std::cout << "Post task from " << i << " to " << i+1000/10 << std::endl;
        thread_pool->post_task(
            [&]{
                fill_in_function( result_set, i, i+1000/10, promise_obj );
            }
        );
    }

    auto check_result_set = [&](){
        std::cout << "Start future_obj.get()" << std::endl;
        ASSERT_TRUE(future_obj.get());
        std::cout << "End future_obj.get()" << std::endl;

        if(future_obj.get())
        {
            for(int i = 0; i < 1000; ++i)
            {
                // BUG: It will block the thread    
                std::cout << i << std::endl;
                ASSERT_EQ(result_set[i], i) << "Error at index: " << i;
            }
        }
        
    };

    thread_pool->post_task( 
        [&]{
            check_result_set();
        }
    );

    for(;;)
    {
        // main thread loop
    }
}