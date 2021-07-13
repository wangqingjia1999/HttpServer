#pragma once

namespace UnixDomainHelper
{
    bool send_fd(const int peer_socket, const int fd);
    int read_fd(const int peer_socket);
};