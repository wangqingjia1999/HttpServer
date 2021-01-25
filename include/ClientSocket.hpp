#pragma once

#include "ISocket.hpp"

#include <vector>
#include <string>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>


class ClientSocket : public ISocket
{
public:
    ClientSocket();
    ~ClientSocket();

public:    
    virtual std::vector<uint8_t>* read_from(const int peer_fd) override;

    virtual bool write_to(const int peer_fd, const std::vector<uint8_t>& data, const int data_size) override;

public:
    /**
     * @brief  Connect to server with the identity of ip:port.
     * @param  ip  Ip address of server.
     * @param  port  Port of server.
     */
    void connect_to(const std::string ip, const int port);

    /**
     * Write to server after successfully connecting to it.
     * 
     * @return 
     *      True if succeeds.
     */
    bool write_to();

    /**
     * Read from server.
     * 
     * @return 
     *      A pointer to socket's internal receive buffer.
     */
    std::vector<uint8_t>* read_from();

    void close_connection();

    int get_client_fd() const;
    
    void fill_send_buffer(const std::string& data_string);

    std::vector<uint8_t>* get_send_buffer();
    std::vector<uint8_t>* get_receive_buffer();
    
    

    
private:
    int client_fd;
    std::vector<uint8_t> receive_buffer;
    std::vector<uint8_t> send_buffer;
};
