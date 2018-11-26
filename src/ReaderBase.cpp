#include "ReaderBase.hpp"

#include <sys/socket.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>


using namespace udp_socket;


ReaderBase::ReaderBase() : ReaderBase(-1, MAX_LENGTH_DEFAULT)
{
}

ReaderBase::ReaderBase(unsigned int port, unsigned int max_length) :
    port(port), buffer_size(max_length), fd_socket(-1),
    socket_created(false), process_last(false)
{
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    buffer = 0;
    buffer_peek = new unsigned char[1];
    buffer_peek[0] = 0;

    setMode(NONBLOCKING | PROCESS_LAST);
}

ReaderBase::~ReaderBase()
{
    deleteBuffer();
    closeSocket();

    delete buffer_peek;
    buffer_peek = 0;
}

void ReaderBase::setPort(unsigned int port)
{
    this->port = port;
    servaddr.sin_port = htons(port);
}

void ReaderBase::setMode(unsigned int mode)
{
    process_last = (mode & 2) == PROCESS_LAST;
    //
    unsigned int bit_block = mode & 1;
    read_flags = (bit_block == NONBLOCKING) ? MSG_DONTWAIT : MSG_WAITALL;
}

void ReaderBase::setAddress(const char *addr)
{
    if (inet_aton(addr, &servaddr.sin_addr) == 0)
    {
        throw std::runtime_error("invalid address");
    }
}

void ReaderBase::clearAddress()
{
    servaddr.sin_addr.s_addr = INADDR_ANY;
}

void ReaderBase::createSocket()
{
    // create socket file descriptor
    if ( (fd_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        throw std::runtime_error("could not create the socket");
    }

    // bind the socket with the server address
    if ( bind(fd_socket, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
    {
        throw std::runtime_error("could not bind the socket");
    }

    createBuffer();

    socket_created = true;
}

void ReaderBase::closeSocket()
{
    if (!socket_created)
        return;

    close(fd_socket);
}

bool ReaderBase::listen()
{
    if (!socket_created)
    {
        std::cerr << "[WARN] socket must be created before call to liten()" << std::endl;
        return false;
    }

    int len;
    bool ret = false;
    // keeps reading until all messages have been read and only the latest one survives
    while (true)
    {
        // read data
        len = recvfrom(fd_socket, buffer, buffer_size, read_flags, NULL, NULL);
        if (len < 0)
            break;

        if (!process_last)
        {
            if (validateData(buffer, len))
                ret |= processData(buffer, len);
        }

        // peek the socket to see if there is more to be read
        if (recvfrom(fd_socket, buffer_peek, 1, MSG_PEEK | MSG_DONTWAIT, NULL, NULL) < 0)
            break;
    }

    if (process_last && len >= 0)
    {
        if (validateData(buffer, len))
            return processData(buffer, len);
        return false;
    }

    return ret;
}
