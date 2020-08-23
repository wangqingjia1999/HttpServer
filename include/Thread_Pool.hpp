#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "Server.hpp"

#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>

class Thread_Pool
{
    // Types
public:
    using Task = std::function< void() >;

    // Life-cycle management
public:
    ~Thread_Pool() noexcept;
    Thread_Pool();

    Thread_Pool(const Thread_Pool&) = delete;
    Thread_Pool& operator=(const Thread_Pool&) = delete;
    
    Thread_Pool(Thread_Pool&&) = delete;
    Thread_Pool& operator=(Thread_Pool&&) = delete;

    // Public methods
public:
    /**
     * @brief  Post task to task queue.
     * @param  new_task  New task.
     */
    void post_task(const Task& new_task);

    /**
     * @brief  Single thread infinite task-loop.
     * 
     * Consistently check whether the task queue has something to fetch.
     */
    void infinite_loop();
    
    /**
     * @brief  Shutdown the thread pool.
     */
    void shutdown_taskers();

    // Private properties
private:
    /**
     * For an efficient thread pool implementation, 
     * once threads are created according to hardware_concurrency(), 
     * it's better not to create new ones, or destroy old ones (by joining).
     */
    const int hardware_supported_threads = std::thread::hardware_concurrency();
    
    /**
     * Thread pool that contains threads in infinite tasking loop.
     */
    std::vector< std::thread > thread_pool;

    /**
     * Mutex for thread pool.
     */
    std::mutex thread_pool_mutex;
    
    /**
     * Task queue based on FIFO principle.
     */
    std::queue< Task > task_queue; 

    /**
     * Mutex for task queue.
     */
    std::mutex task_queue_mutex;

    /**
     * Condition variable for task queue.
     */
    std::condition_variable task_queue_condition;

};

#endif