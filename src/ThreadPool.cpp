#include "ThreadPool.hpp"

struct ThreadPool::Impl
{
    int cpuCores = std::thread::hardware_concurrency();
    

};