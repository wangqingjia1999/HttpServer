#include "Client.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Uri.hpp"

#include <memory>
#include <string>

#include <arpa/inet.h>
#include <error.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

Client::Client() {}

Client::~Client() {}