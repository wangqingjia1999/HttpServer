#ifndef EPOLL_REACTOR_HPP
#define EPOLL_REACTOR_HPP

#include <memory>

class epoll_reactor
{
    public:
    epoll_reactor();
    ~epoll_reactor() noexcept;

    public:

    /**
     * @brief
     *      Interrupt the select loop.
     */ 
    void interrupt();

    /**
     * @brief
     *      Create epoll object.
     * @return 
     *      Success: file descriptor
     *      Error: -1
     */
    int create_epoll();

    /**
     * @brief
     *      Create the timerfd descriptor.
     */
    int create_timerfd();


    private:
    struct Impl;
    std::unique_ptr< Impl > impl_;
};

#endif