#pragma once

#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <future>
#include <condition_variable>
#include <vector>
#include <queue>

class ThreadPool
{
public:
    using Task = std::function< void() >;

public:
    ~ThreadPool() noexcept;
    ThreadPool();

    ThreadPool(const ThreadPool&) = default;
    ThreadPool& operator=(const ThreadPool&) = default;
    
    ThreadPool(ThreadPool&&) = default;
    ThreadPool& operator=(ThreadPool&&) = default;

public:
    /**
     * Post task to task queue.
     * 
     * @param[in] new_task  
     *      New task.
     */
    void post_task(const Task& new_task);

    /**
     * Single thread infinite task-loop.
     * 
     * @note
     *      Consistently check whether the task queue has something to fetch.
     */
    void thread_work_loop();
    
    /**
     * Shutdown the thread pool.
     */
    void shutdown_thread_pool();

    /**
     * Whether the taskes have been finished by workers.
     * 
     * @param[in] thread_index  
     *      Thread index within thread pool.
     * 
     * @return  
     *      True if all taskes have been finished by workers.
     */
    bool have_finished_taskes(int thread_index);

private:
    /**
     * For an efficient thread pool implementation, 
     * once threads are created according to hardware_concurrency(), 
     * it's better not to create new ones, or destroy old ones (by joining).
     */
    const int hardware_supported_threads = std::thread::hardware_concurrency();

    struct thread_entity
    {
        std::thread thread;
        bool has_finished_task;

        thread_entity(std::thread task) : has_finished_task(false) { thread = std::move(task); }
    };
    
    /**
     * Thread pool that contains threads in infinite tasking loop.
     */
    std::vector< thread_entity > thread_pool;
    
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

    /**
     * Wether the thread pool is shutdown.
     */
    bool is_shutdown_thread_pool;

};
