#include "thread_pool.hpp"

/**
 * This class is deprecated :=(
 * Just because I don't know how to implement.
 */
namespace thread_pool
{
    void initialize_thread_pool()
    {
        int hardware_supported_threads = std::thread::hardware_concurrency();
        for(int i = 0; i <= hardware_supported_threads; ++i)
        {
            thread_pool.push_back( std::thread(infinite_work_loop));
        }
    }

    void infinite_work_loop()
    {
        {
            std::unique_lock< std::mutex > lock(work_queue_mutex);
            work_queue_condition.wait(lock, []{ return !work_queue.empty(); });
            auto each_work = work_queue.front();
            work_queue.pop();
            each_work();
        }
    }

    void add_request_handler_work(std::function< bool() > new_request_handler)
    {
        {
            std::unique_lock< std::mutex > lock(work_queue_mutex);
            work_queue.push(new_request_handler);
        }
        work_queue_condition.notify_one();
    }
}