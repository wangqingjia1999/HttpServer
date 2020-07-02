#include "epoll_reactor.hpp"
#include <sys/epoll.h>
#include <sys/timerfd.h>
#include <fcntl.h>


struct epoll_reactor::Impl
{
    int epoll_fd;
    int epoll_size = 20000;
};

int epoll_reactor::create_epoll()
{   
// make sure epoll fd gets closed before executing a new process.
#if defined(EPOLL_CLOEXEC)
    int fd = epoll_create(impl_->epoll_size);
#else 
    int fd = -1;
    errno = EINVAL;
#endif
    if(fd == -1 && (errno == EINVAL || errno == ENOSYS))
    {
        fd = epoll_create(impl_->epoll_size);
        if(fd != -1)
            ::fcntl(fd, F_SETFD, FD_CLOEXEC);
        else
            perror("epoll_create");
    }

    return fd;
}

void epoll_reactor::interrupt()
{
    // epoll_event ev = {0, { 0 } };
    // ev.events = EPOLLIN | EPOLLERR | EPOLLET;
    // ev.data.ptr = 
    // epoll_ctl(impl_->epoll_fd, EPOLL_CTL_MOD, )
}

int epoll_reactor::create_timerfd()
{
#if defined(TFD_CLOEXEC)
    // TFD_CLOEXEC flag is useful for the same reasons as the open() O_CLOEXEC flag
    // CLOCK_MONOTONIC is used for relative time
    int fd = timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC);
#else
    int fa = -1;
    errno = EINVAL;
#endif

    if(fd == -1 && errno == EINVAL)
    {
        fd = timerfd_create(CLOCK_MONOTONIC, 0);
        if(fd != -1)
            ::fcntl(fd, F_SETFD, FD_CLOEXEC);
        else
            perror("timerfd_create");
    }

    return fd;
}