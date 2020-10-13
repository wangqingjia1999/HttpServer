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

    std::vector<int> result_set(10000);   
    std::mutex result_set_mutex;
    std::condition_variable result_set_condition;
    bool is_finished = false;
    bool has_initialized_result_set = false;

    auto initialize_result_set = [&](){
        std::lock_guard< std::mutex > lock(result_set_mutex);
        for(int i = 0; i < 10000; ++i)
        {
            result_set[i] = 0;
        }
        has_initialized_result_set = true;
        std::cout << "Finish initializing result set" << std::endl;
        result_set_condition.notify_all();
    };
    thread_pool->post_task(
        [&]{
            initialize_result_set();
        }
    );
    
    auto fill_in_function = [&](std::vector<int>& result, int begin, int range){
        for(int i = begin; i < begin+range; ++i)
        {
            {
                std::lock_guard< std::mutex > lock(result_set_mutex); 
                result[i] = i;            
            }

            if(i == 999)
                is_finished = true;

            std::cout << i << std::endl;
        }
        result_set_condition.notify_one();
    };
    
    /**
     * Split the taskes to 10 sub-taskes
     */
    for(int i = 0; i < 1000; i += 100 )
    {
        // After has initialized result set, we delegate works.
        std::unique_lock< std::mutex > lock(result_set_mutex);
        result_set_condition.wait(lock, [&]{ return has_initialized_result_set; });
        std::cout << "OK, I will begin to delegate works" << std::endl;
        thread_pool->post_task(
            [&]{
                fill_in_function( result_set, i, 100 );
            }
        );
        lock.unlock();
    }

    auto check_result_set = [&](){
        std::unique_lock< std::mutex > lock(result_set_mutex);
        std::cout << "Let me check the result" << std::endl;
        result_set_condition.wait(lock, [&]{ return is_finished; });
        std::cout << "Ok, we have finished filling the vector" << std::endl;
    };
    thread_pool->post_task(
        [&]{
            check_result_set();
        }
    );
}