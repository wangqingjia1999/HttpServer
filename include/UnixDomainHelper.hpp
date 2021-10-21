#pragma once

namespace UnixDomainHelper
{
	bool send_fd(int socket, int fd);
	int read_fd(int socket);
}; // namespace UnixDomainHelper