#include "VLANInterfaceConfig.h"

#include <cassert>

#include "ConfiguratorData.h"

VLANInterfaceConfig::VLANInterfaceConfig(std::string const &name, ConfiguratorData &configurator, InterfaceConfig *parent_interface, uint16_t vlan_id)
    : InterfaceConfig(name, configurator), parent_interface_(parent_interface), vlan_id_(vlan_id)
{
}

bool VLANInterfaceConfig::UpdateID(uint16_t new_vlan_id)
{
    assert(parent_interface_);
    
    parent_interface_->RemoveVLANInterface(*this);

    vlan_id_ = new_vlan_id;
    if (!InterfaceConfig::Rename(CreateVLANName(parent_interface_->name_, vlan_id_)))
    {
        return false;
    }

    return parent_interface_->AddVLANInterface(*this);
}

bool VLANInterfaceConfig::Remove()
{
    assert(parent_interface_);

    parent_interface_->RemoveVLANInterface(*this);
    parent_interface_ = nullptr;
    return InterfaceConfig::Remove();
}
