#ifndef CONFIGURATOR_DATA_H__
#define CONFIGURATOR_DATA_H__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IPAddr.h"

struct InterfaceConfig;
struct EthernetInterfaceConfig;
struct BridgeConfig;
struct VLANInterfaceConfig;

struct ConfiguratorData
{
    InterfaceConfig *FindInterface(std::string const &name) const;
    EthernetInterfaceConfig *FindEthernetInterface(std::string const &name) const;
    BridgeConfig *FindBridge(std::string const &name) const;
    VLANInterfaceConfig *FindVLANInterface(std::string const &name) const;

    std::unordered_map<std::string, std::unique_ptr<InterfaceConfig>> interfaces_;
    UniqueIPAddresses used_ip_addresses_;
};

#endif // CONFIGURATOR_DATA_H__
