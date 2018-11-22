#pragma once


#include <netinet/in.h>


namespace udp_listener
{


class ListenerBase
{
private:
    unsigned int port, buffer_size;
    struct sockaddr_in servaddr;
    int fd_socket;
    char * buffer;

    bool socket_created;

    void deleteBuffer();

protected:

    virtual bool validateData(const char * const buffer, unsigned int len) = 0;
    virtual bool processData(const char * const buffer, unsigned int len) = 0;

public:
    const static unsigned int MAX_LENGTH_DEFAULT = 1024;

    ListenerBase();
    ListenerBase(unsigned int port = -1, unsigned int max_length = MAX_LENGTH_DEFAULT);
    ~ListenerBase();

    void setPort(unsigned int port);

    /**
     * @brief setAddress Sets a specific address on which to establish a connection
     *
     * If this is not called, the socket binds to all local network interfaces. Therefore, it will
     * accept all UDP packets (also valid for TCP connections, but this class implements UDP)
     * sent to its port, regardless of the network interface on which the requests arrived.
     *
     * @param addr The local interface IPv4 number-and-dots notation address.
     * @see clearAddress
     */
    void setAddress(const char *addr);

    /**
     * @brief clearAddress Clears the address such that the socket binds to all local interfaces.
     *
     * @see setAddress
     */
    void clearAddress();

    void createSocket();
    void listen();
};


}
