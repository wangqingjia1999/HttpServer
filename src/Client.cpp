#include "Uri.hpp"
#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"

#include <memory>
#include <string>

#include <error.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

Client::Client()
{
}

Client::~Client()
{
}