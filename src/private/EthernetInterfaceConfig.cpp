#include "EthernetInterfaceConfig.h"

#include <cassert>

#include "BridgeConfig.h"
#include "ConfiguratorData.h"
#include "VLANInterfaceConfig.h"

EthernetInterfaceConfig::EthernetInterfaceConfig(std::string const &name, ConfiguratorData &configurator, uint32_t MTU, uint32_t speed, DuplexMode duplex_mode)
    : InterfaceConfig(name, configurator), MTU_(MTU), speed_(speed), duplex_mode_(duplex_mode)
{
    interface_type_ = InterfaceTypeValue;
}

bool EthernetInterfaceConfig::Rename(std::string const &new_name)
{
    if (!vlan_interfaces_.empty())
    {
        // For simplicity, disallow renaming if there are VLAN interfaces. Otherwise, we would need to rename all VLAN interfaces as well.
        return false;
    }

    return InterfaceConfig::Rename(new_name);
}

bool EthernetInterfaceConfig::Remove()
{
    if (bridge_ != nullptr)
    {
        return false;
    }
    if (!vlan_interfaces_.empty())
    {
        return false;
    }

    return InterfaceConfig::Remove();
}

bool EthernetInterfaceConfig::AddIPAddress(std::string const &ip_address, uint8_t prefix_length)
{
    return bridge_ == nullptr && InterfaceConfig::AddIPAddress(ip_address, prefix_length);
}

bool EthernetInterfaceConfig::CanBeBridged() const
{
    return bridge_ == nullptr && assigned_ip_addresses_.empty();
}

void EthernetInterfaceConfig::AddToBridge(BridgeConfig &bridge_config)
{
    assert(CanBeBridged());

    bridge_ = &bridge_config;
    auto result = bridge_config.interfaces_.insert(this);
    assert(result.second);
}

void EthernetInterfaceConfig::RemoveFromBridge(BridgeConfig &bridge_config)
{
    assert(bridge_ == &bridge_config);

    bridge_ = nullptr;
    std::size_t erased = bridge_config.interfaces_.erase(this);
    assert(erased == 1);
}

bool EthernetInterfaceConfig::AddVLANInterface(VLANInterfaceConfig const &vlan_interface_config)
{
    assert(vlan_interface_config.parent_interface_ == this);

    auto insert = vlan_interfaces_.insert(vlan_interface_config.vlan_id_);
    return insert.second;
}

void EthernetInterfaceConfig::RemoveVLANInterface(VLANInterfaceConfig const &vlan_interface_config)
{
    std::size_t erased = vlan_interfaces_.erase(vlan_interface_config.vlan_id_);
    assert(erased == 1);
}
