#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <memory>
#include <functional>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>

namespace thread_pool
{
    static std::mutex thread_pool_mutex;
    static std::mutex work_queue_mutex;

    std::condition_variable thread_pool_condition;
    std::condition_variable work_queue_condition;

    std::vector< std::thread > thread_pool;
    std::queue< std::function< bool() > > work_queue;

    void infinite_work_loop();
    void add_request_handler_work(std::function< bool() > new_request_handler);
    void initialize_thread_pool();
    void close_thread_pool();
}

#endif