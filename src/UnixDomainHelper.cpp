#include "UnixDomainHelper.hpp"
#include "Logger.hpp"

#include <sys/socket.h>
#include <sys/types.h>

namespace UnixDomainHelper
{
    bool send_fd(const int peer_socket, const int fd)
    {
        char data_buffer[] = " ";
        struct iovec io_vector = {
            .iov_base = &data_buffer,
            .iov_len = sizeof(data_buffer)
        };

        union
        {
            char control_message_buffer[CMSG_SPACE(sizeof(fd))];
            struct cmsghdr control_message;
        } control_message;

        struct msghdr message = {
            .msg_iov = &io_vector,
            .msg_iovlen = 1,
            .msg_control = control_message.control_message_buffer,
            .msg_controllen = sizeof(control_message.control_message_buffer)
        };

        struct cmsghdr* first_control_message = CMSG_FIRSTHDR(&message);
        first_control_message->cmsg_len = CMSG_LEN(sizeof(fd));
        first_control_message->cmsg_level = SOL_SOCKET;
        first_control_message->cmsg_type = SCM_RIGHTS;

        memcpy(CMSG_DATA(first_control_message), &fd, sizeof(fd));

        if(sendmsg(peer_socket, &message, 0) != 0)
        {
            Logger::error("sendmsg() in master error: " + std::string{ strerror(errno) });
            return false;
        }
        
        return true;
    }

    int read_fd(const int peer_socket)
    {
        struct msghdr message = { 0 };

        char data_buffer[2];

        struct iovec io_vector = {
            .iov_base = data_buffer,
            .iov_len = sizeof(data_buffer)
        };

        message.msg_iov = &io_vector;
        message.msg_iovlen = 1;

        char control_message_buffer[sizeof(int)];
        message.msg_control = &control_message_buffer;
        message.msg_controllen = sizeof(control_message_buffer);

        if(recvmsg(peer_socket, &message, 0) == -1)
        {
            Logger::error("recvmsg() error: " + std::string{ strerror(errno) });
            return -1;
        }

        struct cmsghdr* control_message = CMSG_FIRSTHDR(&message);
        int accept_event_fd = static_cast<int>( *(CMSG_DATA(control_message)) );
        delete control_message;
        control_message = nullptr;
        return accept_event_fd;
    }
};