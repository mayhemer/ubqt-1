#ifndef IPADDR_H__
#define IPADDR_H__

#include <string>
#include <unordered_set>
#include <unordered_map>

class IPAddress
{
public:
    static bool IsValid(const std::string &address, uint8_t &max_prefix_length);
};

using UniqueIPAddresses = std::unordered_set<std::string>;
using UniquePrefixedIPAddresses = std::unordered_map<std::string, uint8_t>;

#endif // IPADDR_H__
