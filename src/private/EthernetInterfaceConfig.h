#ifndef ETHERNET_INTERFACE_CONFIG_H__
#define ETHERNET_INTERFACE_CONFIG_H__

#include "Interface.h"

#include <string>
#include <unordered_map>

class BridgeConfig;

struct EthernetInterfaceConfig : public InterfaceConfig
{
    EthernetInterfaceConfig() = delete;
    EthernetInterfaceConfig(std::string const &name, ConfiguratorData &configurator, uint32_t MTU, uint32_t speed, bool fullduplex);

    virtual InterfaceType GetType() const override { return InterfaceType::Ethernet; }

    virtual bool Rename(std::string const &new_name) override;
    virtual bool Remove() override;

    virtual bool AddIPAddress(std::string const &ip_address) override;

    virtual bool CanBeBridged() const override;
    virtual void AddToBridge(BridgeConfig &bridge_config) override;
    virtual void RemoveFromBridge(BridgeConfig &bridge_config) override;

    virtual bool AddVLANInterface(VLANInterfaceConfig &vlan_interface_config) override;
    virtual void RemoveVLANInterface(VLANInterfaceConfig &vlan_interface_config) override;

    uint32_t MTU_;
    uint32_t speed_;
    bool fullduplex_;

    InterfaceConfig *bridge_{nullptr};
    std::unordered_set<uint16_t> vlan_interfaces_;
};

#endif
