#include "WorkerSocket.hpp"
#include "ClientSocket.hpp"

#include <gtest/gtest.h>

TEST(server_socket_tests, default_constructor_no_throw_test)
{
    EXPECT_NO_THROW(WorkerSocket ss{});
}