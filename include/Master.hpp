#pragma once

#include "Channel.hpp"
#include "Connection.hpp"

#include <queue>
#include <unordered_set>

#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace Master
{
	void listen_at(const std::string& ip, int port);
	void clear_up();
}; // namespace Master
