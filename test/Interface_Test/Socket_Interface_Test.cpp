#include "ISocket.hpp"
#include "Server_Socket.hpp"
#include "Client_Socket.hpp"
#include "Thread_Pool.hpp"

#include <gtest/gtest.h>
#include <memory>
#include <iostream>

// Time units for testing
const u_int64 microseconds = 1;
const u_int64 milliseconds = 1000 * microseconds;
const u_int64 seconds = 1000 * milliseconds;

