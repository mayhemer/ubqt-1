#ifndef VLAN_INTERFACE_CONFIG_H__
#define VLAN_INTERFACE_CONFIG_H__

#include <string>

#include "Interface.h"

struct VLANInterfaceConfig : public InterfaceConfig
{
    inline static constexpr InterfaceType InterfaceTypeValue = InterfaceType::VLAN;

    VLANInterfaceConfig() = delete;
    VLANInterfaceConfig(std::string const &name, ConfiguratorData &configurator, InterfaceConfig *parent_interface, uint16_t vlan_id);

    inline static std::string CreateVLANName(std::string const &parent_interface, uint16_t ID)
    {
        return parent_interface + "." + std::to_string(ID);
    }

    virtual bool Rename(std::string const &new_name) override { return false; }
    virtual bool Remove() override;

    bool UpdateID(uint16_t new_vlan_id);

    InterfaceConfig *parent_interface_{nullptr};
    uint16_t vlan_id_{0};
};

#endif // VLAN_INTERFACE_CONFIG_H__
