#include "Server.hpp"
#include "Client.hpp"
#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <string>

int main()
{
    Server server;

    server.listen_at("127.0.0.1", 2333);
}