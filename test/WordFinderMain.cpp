#include "Master.hpp"
#include <sys/resource.h>
#include <sys/signal.h>

void daemonize()
{
    pid_t pid;
    struct rlimit rl;
    struct sigaction sa;

    umask(0);

    if(getrlimit(RLIMIT_NOFILE, &rl) < 0)
    {
        Logger::error("can't get file limit");
        return;
    }

    // become a session leader to lose controlling TTY.
    if((pid = fork()) < 0)
    {
        Logger::error("can't fork");
        return;
    }
    else if(pid != 0) // parent
    {
        // terminate parent process normally to ensure the 
        // child process is not the process group leader,
        // so that the call to setsid() will not fail.
        exit(0);
    }

    setsid();

    // ensure future opens won't allocate controlling TTYs.
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if(sigaction(SIGHUP, &sa, NULL) < 0)
    {
        Logger::error("can't ignore SIGHUP");
        return;
    }

    if((pid = fork()) < 0)
    {
        Logger::error("can't fork");
        return;
    }
    else if(pid != 0) // parent
    {
        exit(0);
    }

    // change current working directory to the root 
    // so we won't prevent file system from being unmounted.
    if(chdir("/home/HttpServer") < 0)
    {
        Logger::error("can't change directory");
        return;
    }

    // close all open file descriptions
    if(rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024;
    for(int i = 0; i < rl.rlim_max; ++i)
        close(i);

    Logger::info("Finish daemonizing, server runs in process " + std::to_string(getpid()));
}

int main()
{
    daemonize();
    
    try
    {
        Master master("0.0.0.0", 80);
    }
    catch(const std::exception& e)
    {
        Logger::error("exception: " + std::string{e.what()});
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}