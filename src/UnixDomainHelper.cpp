#include "UnixDomainHelper.hpp"
#include "Logger.hpp"

#include <sys/socket.h>
#include <sys/types.h>

namespace UnixDomainHelper
{
	bool send_fd(int socket, int fd)
	{
		char data_buffer[] = " ";
		struct iovec io_vector = {.iov_base = &data_buffer,
		                          .iov_len = sizeof(data_buffer)};

		union
		{
			char control_message_buffer[CMSG_SPACE(sizeof(fd))];
			struct cmsghdr control_message;
		} control_message;

		struct msghdr message = {
		    .msg_iov = &io_vector,
		    .msg_iovlen = 1,
		    .msg_control = control_message.control_message_buffer,
		    .msg_controllen = sizeof(control_message.control_message_buffer)};

		struct cmsghdr* first_control_message = CMSG_FIRSTHDR(&message);
		first_control_message->cmsg_len = CMSG_LEN(sizeof(fd));
		first_control_message->cmsg_level = SOL_SOCKET;
		first_control_message->cmsg_type = SCM_RIGHTS;

		memcpy(CMSG_DATA(first_control_message), &fd, sizeof(fd));

		if (sendmsg(socket, &message, 0) == -1)
		{
			Logger::error("sendmsg() error", errno);
			return false;
		}

		return true;
	}

	int read_fd(int socket)
	{
		struct msghdr message = {nullptr};

		char data_buffer[256]; // NOLINT

		struct iovec io_vector = {.iov_base = data_buffer,
		                          .iov_len = sizeof(data_buffer)};

		message.msg_iov = &io_vector;
		message.msg_iovlen = 1;

		char control_message_buffer[256]; // NOLINT
		message.msg_control = &control_message_buffer;
		message.msg_controllen = sizeof(control_message_buffer);

		int length = recvmsg(socket, &message, MSG_DONTWAIT);
		if (length !=
		    2) // because sendmsg will sends exactly 2 bytes as payload
		{
			Logger::error("recvmsg() error", errno);
			return -1;
		}

		struct cmsghdr* control_message = CMSG_FIRSTHDR(&message);
		unsigned char* data = CMSG_DATA(control_message);
		return *((int*)data);
	}
}; // namespace UnixDomainHelper