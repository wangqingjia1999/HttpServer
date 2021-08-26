#pragma once

#include "Connection.hpp"
#include "Channel.hpp"

#include <queue>
#include <unordered_set>

#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace Master
{
    void listen_at(const std::string& ip, const int port);
    void clear_up();
};
