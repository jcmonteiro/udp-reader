#pragma once


#include <netinet/in.h>
#include <cstring>


namespace udp_socket
{


/**
 * @brief The ListenerBase class implements a UDP listener that reads only the latest message
 */
class ReaderBase
{
public:
    const static unsigned int MAX_LENGTH_DEFAULT = 1024;

    enum Mode
    {
        NONBLOCKING = 1,
        BLOCKING = 0,
        PROCESS_LAST = 2,
        PROCESS_ALL = 0
    };

private:
    unsigned int port, buffer_size;
    struct sockaddr_in servaddr;
    int fd_socket;
    unsigned char *buffer, *buffer_peek;
    int read_flags;

    bool socket_created, process_last;

    inline void deleteBuffer()
    {
        if (buffer == 0)
            return;
        delete buffer;
        buffer = 0;
    }

    inline void createBuffer()
    {
        deleteBuffer();
        buffer = new unsigned char[buffer_size];
        std::memset(buffer, 0, buffer_size * sizeof(char));
    }

protected:
    virtual bool validateData(const unsigned char * const buffer, unsigned int len) = 0;
    virtual bool processData(const unsigned char * const buffer, unsigned int len) = 0;

public:
    ReaderBase();
    ReaderBase(unsigned int port, unsigned int max_length);
    virtual ~ReaderBase();

    inline void setMaximumMessageLength(unsigned int max_length)
    {
        buffer_size = max_length;
        if (socket_created)
            createBuffer();
    }

    /**
     * @brief setPort Sets the port on which to wait for messages.
     * @param port The port number.
     */
    void setPort(unsigned int port);

    /**
     * @brief setMode Sets the read mode, e.g. mode NONBLOCKING | PROCESS_LAST.
     * @param mode The read mode.
     */
    void setMode(unsigned int mode);

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
    void closeSocket();

    /**
     * @brief listen Returns true if ANY one of the reads is successful and false otherwise (ALL fail)
     * @return The processing status
     */
    bool listen();
};


}
