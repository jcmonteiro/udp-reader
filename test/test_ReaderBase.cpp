#define BOOST_TEST_MODULE ListenerBase test
#include <boost/test/unit_test.hpp>

#include "ReaderBase.hpp"

#include <iostream>
#include <unistd.h>

using namespace udp_socket;

class SimpleListener : public ReaderBase
{
protected:
    bool validateData(const unsigned char * const, unsigned int)
    {
        std::cout << "validate" << std::endl;
        return true;
    }

    bool processData(const unsigned char * const buffer, unsigned int len)
    {
        std::printf("process (%d %s): %.*s\n", len, (len > 1) ? "bytes" : "byte", len, buffer);
        return true;
    }
public:
    SimpleListener(unsigned int port, unsigned int max_length) : ReaderBase(port, max_length)
    {
    }
};

BOOST_AUTO_TEST_CASE( test_listener )
{
    const unsigned int sleep_secs = 10;
    const unsigned int port = 3000;
    const unsigned int max_length = 5;
    std::cout << "This test waits for " << sleep_secs
              << " seconds before listening to the socket on port " << port << ".\n"
              << "Consider sending messages via echo -n \"message\" >/dev/udp/localhost/"
              << port << ".\nThe maximum message size is " << max_length << ".\n";

    SimpleListener listener(port, max_length);
    listener.setPort(port);
    listener.setMode(ReaderBase::BLOCKING);

    listener.createSocket();
    usleep(1000 * 1000 * sleep_secs); // microsec
    listener.listen();
    listener.closeSocket();
}
