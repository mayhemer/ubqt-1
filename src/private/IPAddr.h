#ifndef IPADDR_H__
#define IPADDR_H__

#include <string>
#include <unordered_set>

class IPAddress
{
public:
    static bool IsValid(const std::string &address);
};

using UniqueIPAddresses = std::unordered_set<std::string>;

#endif // IPADDR_H__
