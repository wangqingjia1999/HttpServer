#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <vector>
#include <string>

/**
 * TODO: Socket interface class.
 */
class ISocket
{
public:
    /**
     * Virtual deconstructor to ensure proper cleanup.
     */
    virtual ~ISocket() {};

    /**
     * Read from peer.
     * 
     * @return
     *      Reference to data.
     */
    virtual std::vector<uint8_t>* read_from(const int peer_fd) = 0;
    
    /**
     * Write to peer
     * 
     * @param[in] data
     *      Data to be sent.
     * 
     * @param[in] data_size
     *      Data size in byte.
     * 
     * @return 
     *      True if succeeds.
     */
    virtual bool write_to(const int peer_fd, const std::vector<uint8_t>& data, const int data_size) = 0;
};


#endif