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
    // Life-cycle management
public:
    ~Thread_Pool() noexcept;
    Thread_Pool();

    Thread_Pool(const Thread_Pool&) = delete;
    Thread_Pool& operator=(const Thread_Pool&) = delete;
    
    Thread_Pool(Thread_Pool&&) = delete;
    Thread_Pool& operator=(Thread_Pool&&) = delete;

    // Types
private:
    using Work = std::function< void() >;

    // Public methods
public:
    /**
     * @brief  Add work to work queue.
     * @param  new_work  New work.
     */
    void add_work(Work new_work);

    /**
     * @brief  Single thread infinite work-loop.
     * 
     * Consistently check whether the work queue has something to fetch.
     */
    void infinite_loop();
    
    /**
     * @brief  Shutdown the thread pool.
     */
    void shutdown_workers();

    // Private properties
private:
    /**
     * For an efficient thread pool implementation, 
     * once threads are created according to hardware_concurrency(), 
     * it's better not to create new ones, or destroy old ones (by joining).
     */
    const int hardware_supported_threads = std::thread::hardware_concurrency();
    
    /**
     * Thread pool that contains threads in infinite working loop.
     */
    std::vector< std::thread > thread_pool;

    /**
     * Mutex for thread pool.
     */
    std::mutex thread_pool_mutex;
    
    /**
     * Work queue based on FIFO principle.
     */
    std::queue< std::function< void () >> work_queue; 

    /**
     * Mutex for work queue.
     */
    std::mutex work_queue_mutex;

    /**
     * Condition variable for work queue.
     */
    std::condition_variable work_queue_condition;

};

#endif