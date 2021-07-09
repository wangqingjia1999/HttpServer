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


void terniamtion_handler(int signum)
{
    std::cout << "custom handler gets called" << std::endl;
    signal(signum, SIG_DFL);
}

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
                struct sigaction new_action;
                new_action.sa_flags = SA_RESTART;
                new_action.sa_handler = terniamtion_handler;
                sigemptyset(&new_action.sa_mask);

                sigaction(SIGTERM, &new_action, NULL);

                for(;;)
                {
                }
            }

            default:
            {
                children.push_back(child);
                exit;
            }
        }
    }

    for(const auto& c : children)
    {
        kill(c, SIGTERM);
    }

    wait(NULL);

    exit(EXIT_SUCCESS);
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