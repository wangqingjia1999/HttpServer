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
        Work work;
        {
            std::unique_lock< std::mutex > lock(work_queue_mutex);

            work_queue_condition.wait( lock, [this]{ return !this->work_queue.empty(); } );
            work = work_queue.front();
            work_queue.pop();
        }
        work();
    }
}

void Thread_Pool::add_work(Work new_work)
{
    {
        std::unique_lock< std::mutex > lock(work_queue_mutex);
        work_queue.push(new_work);
    }
    work_queue_condition.notify_one();
}

void Thread_Pool::shutdown_workers()
{
    std::unique_lock< std::mutex > lock( thread_pool_mutex );

    for(std::thread& each_thread : thread_pool)
    {
        each_thread.join();
    }
    thread_pool.clear();
}