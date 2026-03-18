#include "IPAddr.h"

#include <arpa/inet.h>

bool IPAddress::IsValid(const std::string &address)
{
    struct in_addr addr;
    if (inet_pton(AF_INET, address.c_str(), &addr) == 1)
    {
        return true;
    }

    struct in6_addr addr6;
    if (inet_pton(AF_INET6, address.c_str(), &addr6) == 1)
    {
        return true;
    }

    return false;
}
