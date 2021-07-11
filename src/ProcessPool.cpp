#include "ProcessPool.hpp"
#include "Worker.hpp"

#include <vector>
#include <exception>
#include <iostream>

#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>

ProcessPool::ProcessPool()
{
    m_cpu_cores = get_nprocs(); 
    
    std::vector<pid_t> children;

    for(int i = 0; i < m_cpu_cores; ++i)
    {
        pid_t child;
        switch(child = fork())
        {
            case -1:
            {
                throw std::runtime_error("can't fork worker process");
            }

            case 0: 
            {
                
                pause();
            }

            default:
            {
                children.push_back(child);
                break;
            }
        }
    }
}

ProcessPool::~ProcessPool()
{
    
}


int ProcessPool::get_cpu_cores() const
{
    return m_cpu_cores;
}

void ProcessPool::close_pool()
{
    
    
}