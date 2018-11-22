#include "listenerbase.hpp"

#include <sys/socket.h>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>


using namespace udp_listener;


ListenerBase::ListenerBase(unsigned int port, unsigned int max_length) :
    port(port), buffer_size(max_length), fd_socket(-1),
    socket_created(false)
{
    std::memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
}

ListenerBase::~ListenerBase()
{
    deleteBuffer();
}

void ListenerBase::setPort(unsigned int port)
{
    this->port = port;
    servaddr.sin_port = htons(port);
}

void ListenerBase::setAddress(const char *addr)
{
    if (inet_aton(addr, &servaddr.sin_addr) == 0)
    {
        throw std::runtime_error("invalid address");
    }
}

void ListenerBase::clearAddress()
{
    servaddr.sin_addr.s_addr = INADDR_ANY;
}

void ListenerBase::createSocket()
{
    // create socket file descriptor
    if ( (fd_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 )
    {
        throw std::runtime_error("could not create a socket");
    }

    buffer = new char[buffer_size];
    std::memset(buffer, 0, buffer_size * sizeof(char));

    socket_created = true;
}

void ListenerBase::listen()
{
    unsigned int len = recvfrom(fd_socket, buffer, buffer_size,
                        MSG_WAITALL, NULL, NULL);
    validateData(buffer, len);
    processData(buffer, len);
}

void ListenerBase::deleteBuffer()
{
    if (buffer == 0)
        return;
    delete buffer;
    buffer = 0;
}
