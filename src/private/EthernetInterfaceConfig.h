#ifndef ETHERNET_INTERFACE_CONFIG_H__
#define ETHERNET_INTERFACE_CONFIG_H__

#include "Interface.h"

#include <string>
#include <unordered_map>

class BridgeConfig;

struct EthernetInterfaceConfig : public InterfaceConfig
{
    inline static constexpr InterfaceType InterfaceTypeValue = InterfaceType::Ethernet;

    enum class DuplexMode
    {
        AutoNeg,
        Half,
        Full
    };

    EthernetInterfaceConfig() = delete;
    EthernetInterfaceConfig(std::string const &name, ConfiguratorData &configurator, uint32_t MTU, uint32_t speed, DuplexMode duplex_mode);

    virtual bool Rename(std::string const &new_name) override;
    virtual bool Remove() override;

    virtual bool AddIPAddress(std::string const &ip_address, uint8_t prefix_length) override;

    virtual bool CanBeBridged() const override;
    virtual void AddToBridge(BridgeConfig &bridge_config) override;
    virtual void RemoveFromBridge(BridgeConfig &bridge_config) override;

    virtual bool AddVLANInterface(VLANInterfaceConfig const &vlan_interface_config) override;
    virtual void RemoveVLANInterface(VLANInterfaceConfig const &vlan_interface_config) override;

    uint32_t MTU_;
    uint32_t speed_;
    DuplexMode duplex_mode_;

    InterfaceConfig *bridge_{nullptr};
    std::unordered_set<uint16_t> vlan_interfaces_;
};

#endif
