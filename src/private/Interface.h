#ifndef ADDRESSIBLE_INTERFACE_H__
#define ADDRESSIBLE_INTERFACE_H__

#include "IPAddr.h"

#include <cassert>
#include <string>
#include <unordered_map>

class ConfiguratorData;

struct EthernetInterfaceConfig;
struct BridgeConfig;
struct VLANInterfaceConfig;

enum class InterfaceType
{
    Ethernet,
    Bridge,
    VLAN
};

struct InterfaceConfig
{
    InterfaceConfig() = delete;
    InterfaceConfig(std::string const &name, ConfiguratorData &configurator);
    virtual ~InterfaceConfig() = default;

    virtual InterfaceType GetType() const = 0;

    virtual bool SetAdminState(bool up);
    virtual bool Rename(std::string const &new_name);

    virtual bool Remove();

    virtual bool AddIPAddress(std::string const &ip_address);
    virtual bool RemoveIPAddress(std::string const &ip_address);

    virtual bool CanBeBridged() const { return false; }
    virtual void AddToBridge([[maybe_unused]] BridgeConfig &bridge_config) { assert(false); }
    virtual void RemoveFromBridge([[maybe_unused]] BridgeConfig &bridge_config) { assert(false); }

    virtual bool AddVLANInterface([[maybe_unused]] VLANInterfaceConfig &vlan_interface_config) { return false; }
    virtual void RemoveVLANInterface([[maybe_unused]] VLANInterfaceConfig &vlan_interface_config) { assert(false); }

    ConfiguratorData &configurator_;
    std::string name_;
    UniqueIPAddresses assigned_ip_addresses_{};
    bool is_up_{false};
};

#endif
