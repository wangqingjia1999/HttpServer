#pragma once

#include <sys/types.h>

class Channel
{
public:
    Channel(const pid_t pid, const int socket);

public:
    void set_worker_pid(const pid_t pid);
    void set_worker_socket(const int socket);

    int get_worker_socket();
    pid_t get_worker_pid();

private:
    int m_worker_socket;
    pid_t m_worker_pid;
};