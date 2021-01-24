## When is the EPOLLHUP triggered?

Use **tcpdump** to trace down into TCP transactions.


## Why there are so many CLOSE_WAIT tcp sockets in server side and so many FIN_WAIT_2 tcp sockets in client side?

To cut a long story short, those who in CLOSE_WAIT did not properly close the TCP connection. In my case, the server failed to close the connection after no more data to read/send.


If client call shutdown(SHUT_WR), the server will receive an EOF from read() system call. In TCP level, the server will receive a FIN from client. After the server's tcp send back an ACK. The server is in CLOSE_WAIT and client is in FIN_WAIT_2.

## IS EPOLLRDDUP equal to read() return 0, which means I do not need to handle the return value in read() call?
If EPOLLRDHUP got triggered, do not read the socket but close it immediately. However, it's possible that, before you call read(), a FIN arrives. So to be safer, close the socket with the return value of 0 of read().