#include "UnixDomainHelper.hpp"

#include <gtest/gtest.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstdio>

TEST(unix_domain_helper_tests, demo)
{
    int fds[2];
    
    EXPECT_EQ(socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0, fds), 0);

    int fd = open("/dev/null", O_WRONLY);

    EXPECT_GT(fd, 0);

    switch(fork())
    {
        case -1:
        {
            break;
        }

        case 0:
        {
            EXPECT_TRUE(UnixDomainHelper::send_fd(fds[1], fd));
            printf("child sends fd: %d\n", fd);
            exit(EXIT_SUCCESS);
            break;
        }

        default:
        {
            break;
        }
    }
    
    wait(NULL);
    int received_fd = 0;
    received_fd = UnixDomainHelper::read_fd(fds[0]);
    EXPECT_GT(received_fd, 0);
    printf("parent receives fd: %d\n", received_fd);
    close(fd);
    exit(EXIT_SUCCESS);
}
