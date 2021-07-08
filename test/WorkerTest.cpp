#include "Worker.hpp"

#include <gtest/gtest.h>

#include <unistd.h>
#include <sys/types.h>

#include <iostream>
#include <memory>
#include <vector>

TEST(worker_tests, experiment)
{
    int CPU_CORES = 2;
    
    std::vector<pid_t>* children = new std::vector<pid_t>(CPU_CORES);

    for(int i = 0; i < CPU_CORES; ++i)
    {
        switch(fork())
        {
            case -1:
            {
                exit(EXIT_FAILURE);
            }

            case 0:
            {
                children->push_back(getpid());
                std::cout << "child pid: " << getpid() << std::endl;
                for(;;)
                {

                }
                break;
            }

            default:
            {
                break;
            }
        }
    }

    for(int i = 0; i < CPU_CORES; ++i)
    {
        std::cout << "child pid: " << children->at(i) << std::endl;
    }
}

TEST(worker_tests, shared_memory_cpp_allocator_test)
{
    
}