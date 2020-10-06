#include "Thread_Pool.hpp"

Thread_Pool::Thread_Pool() : is_shutdown_thread_pool(false) 
{
    for(int i = 0; i < hardware_supported_threads; ++i)
    {
        thread_pool.push_back( std::thread( [this]{ this->thread_work_loop(); } ) );
    }
}

Thread_Pool::~Thread_Pool() = default;

void Thread_Pool::thread_work_loop()
{
    for(;;)
    {
        Task task;
        
        {
            std::unique_lock< std::mutex > lock(task_queue_mutex);
            
            task_queue_condition.wait( lock, [this]{ return !this->task_queue.empty() || is_shutdown_thread_pool; } );

            if(is_shutdown_thread_pool && task_queue.empty())
            {
                break;
            }

            task = std::move( task_queue.front() );
            
            task_queue.pop();

            task();
        }
        
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

void Thread_Pool::shutdown_thread_pool()
{
    {
        std::unique_lock< std::mutex > lock( task_queue_mutex );
        is_shutdown_thread_pool = true;
    }
    
    task_queue_condition.notify_all();

    for(std::thread& each_thread : thread_pool)
    {
        each_thread.join();
    }
    thread_pool.clear();   
}