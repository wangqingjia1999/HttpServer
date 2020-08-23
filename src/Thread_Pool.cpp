#include "Thread_Pool.hpp"

#include <iostream>

Thread_Pool::Thread_Pool()
{
    for(int i = 0; i < hardware_supported_threads; ++i)
    {
        thread_pool.push_back( std::thread( [this]{ this->infinite_loop(); } ) );
    }
}

Thread_Pool::~Thread_Pool()
{
    for(std::thread& each_thread : thread_pool)
    {
        each_thread.join();
    }
    thread_pool.clear();
}

void Thread_Pool::infinite_loop()
{
    while(true)
    {
        Task task;
        {
            std::unique_lock< std::mutex > lock(task_queue_mutex);

            task_queue_condition.wait( lock, [this]{ return !this->task_queue.empty(); } );
            task = std::move( task_queue.front() );
            task_queue.pop();
        }
        task();
    }
}

void Thread_Pool::post_task(const Task& new_task)
{
    {
        std::unique_lock< std::mutex > lock(task_queue_mutex);
        task_queue.push(new_task);
    }
    task_queue_condition.notify_one();
}

void Thread_Pool::shutdown_taskers()
{
    std::unique_lock< std::mutex > lock( thread_pool_mutex );

    for(std::thread& each_thread : thread_pool)
    {
        each_thread.join();
    }
    thread_pool.clear();
}

