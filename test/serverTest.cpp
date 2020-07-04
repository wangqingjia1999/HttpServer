#include <gtest/gtest.h>
#include "Uri.hpp"
#include "Response.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Client.hpp"

#include <thread>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>




