#include "Worker.hpp"

#include <gtest/gtest.h>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>
#include <memory>
#include <vector>
#include <sys/mman.h>

void* create_shared_memory(size_t size)
{
    int protection = PROT_READ | PROT_WRITE;

    int visibility = MAP_SHARED | MAP_ANONYMOUS;

    return mmap(NULL, size, protection, visibility, -1, 0);
}

TEST(worker_tests, shared_memory_test)
{
    void* buffer = create_shared_memory(1024);
    
    switch(fork())
    {
        case -1:
        {
            
        }

        case 0:
        {
            
        }

        default:
        {
            break;
        }
    }

}