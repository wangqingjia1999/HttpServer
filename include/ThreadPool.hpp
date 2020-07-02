#pragma once


#include <thread>

class ThreadPool
{


private:
    struct Impl;
    std::unique_ptr< Impl > impl_;
};