#include "IPAddr.h"

#include <arpa/inet.h>

bool IPAddress::IsValid(const std::string &address, uint8_t &max_prefix_length)
{
    struct in_addr addr;
    if (inet_pton(AF_INET, address.c_str(), &addr) == 1)
    {
        max_prefix_length = 32;
        return true;
    }

    struct in6_addr addr6;
    if (inet_pton(AF_INET6, address.c_str(), &addr6) == 1)
    {
        max_prefix_length = 128;
        return true;
    }

    return false;
}
