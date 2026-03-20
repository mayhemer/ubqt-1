#include "Configurator.h"

#include "BridgeConfig.h"
#include "EthernetInterfaceConfig.h"
#include "VLANInterfaceConfig.h"

bool Configurator::IsUniqueInterface(std::string const &name) const
{
    return conf_.interfaces_.find(name) == conf_.interfaces_.end();
}

Configurator &Configurator::Get()
{
    static Configurator self;
    return self;
}

bool Configurator::AddEthernetInteface(std::string const &name, int speed, int MTU, EthernetInterfaceConfig::DuplexMode duplex_mode)
{
    if (!IsUniqueInterface(name))
    {
        return false;
    }

    auto ethernet_interface = std::make_unique<EthernetInterfaceConfig>(name, conf_, speed, MTU, duplex_mode);
    conf_.interfaces_[name] = std::move(ethernet_interface);
    return true;
}

bool Configurator::BuildBridge(std::string const &name, std::vector<std::string> &&interfaces)
{
    if (!IsUniqueInterface(name))
    {
        return false;
    }

    std::unordered_set<InterfaceConfig *> interfaces_to_add_to;
    interfaces_to_add_to.reserve(interfaces.size());
    for (auto const &interface_name : interfaces)
    {
        auto interface = conf_.FindInterface(interface_name);
        if (!interface)
        {
            return false;
        }
        if (!interface->CanBeBridged())
        {
            return false;
        }
        interfaces_to_add_to.insert(interface);
    }

    if (interfaces_to_add_to.size() < 2)
    {
        return false;
    }

    auto bridge = std::make_unique<BridgeConfig>(name, conf_, false);
    for (auto &interface : interfaces_to_add_to)
    {
        interface->AddToBridge(*bridge);
    }

    conf_.interfaces_[name] = std::move(bridge);
    return true;
}

bool Configurator::AddVLAN(std::string const &parent_interface, int ID)
{
    if (ID < 1 || ID > 4094)
    {
        return false;
    }

    std::string vlan_interface_name = VLANInterfaceConfig::CreateVLANName(parent_interface, ID);
    if (!IsUniqueInterface(vlan_interface_name))
    {
        return false;
    }

    auto parent = conf_.FindInterface(parent_interface);
    if (!parent)
    {
        return false;
    }

    auto VLAN = std::make_unique<VLANInterfaceConfig>(vlan_interface_name, conf_, parent, ID);
    if (!parent->AddVLANInterface(*VLAN))
    {
        return false;
    }

    conf_.interfaces_[vlan_interface_name] = std::move(VLAN);
    return true;
}

bool Configurator::RemoveInteface(std::string const &name)
{
    auto interface = conf_.FindInterface(name);
    if (!interface)
    {
        return false;
    }

    if (!interface->Remove())
    {
        return false;
    }

    conf_.interfaces_.erase(name);
    return true;
}

bool Configurator::AddIPAddress(std::string const &interface_name, std::string const &ip_address, uint8_t prefix_length)
{
    auto interface = conf_.FindInterface(interface_name);
    if (!interface)
    {
        return false;
    }

    return interface->AddIPAddress(ip_address, prefix_length);
}

bool Configurator::RemoveIPAddress(std::string const &interface_name, std::string const &ip_address)
{
    auto interface = conf_.FindInterface(interface_name);
    if (!interface)
    {
        return false;
    }

    return interface->RemoveIPAddress(ip_address);
}

bool Configurator::RenameInterface(std::string const &interface_name, std::string const &new_name)
{
    auto interface = conf_.FindInterface(interface_name);
    if (!interface)
    {
        return false;
    }

    return interface->Rename(new_name);
}

bool Configurator::UpdateAdminState(std::string const &interface_name, bool up)
{
    auto interface = conf_.FindInterface(interface_name);
    if (!interface)
    {
        return false;
    }

    return interface->SetAdminState(up);
}

bool Configurator::UpdateBridgeSTP(std::string const &bridge_name, bool STP)
{
    auto bridge = conf_.FindInterfaceOfType<BridgeConfig>(bridge_name);
    if (!bridge)
    {
        return false;
    }

    return bridge->STP_ = STP;
}

bool Configurator::UpdateBridgeAddInterfaces(std::string const &bridge_name, std::vector<std::string> &&interfaces)
{
    auto bridge = conf_.FindInterfaceOfType<BridgeConfig>(bridge_name);
    if (!bridge)
    {
        return false;
    }

    std::unordered_set<InterfaceConfig *> interfaces_to_add_to;
    interfaces_to_add_to.reserve(interfaces.size());
    for (auto const &interface_name : interfaces)
    {
        auto interface = conf_.FindInterface(interface_name);
        if (!interface)
        {
            return false;
        }
        if (!interface->CanBeBridged())
        {
            return false;
        }
        interfaces_to_add_to.insert(interface);
    }

    if (!interfaces_to_add_to.size())
    {
        return false;
    }

    for (auto &interface : interfaces_to_add_to)
    {
        interface->AddToBridge(*bridge);
    }

    return true;
}

bool Configurator::UpdateBridgeRemoveInterfaces(std::string const &bridge_name, std::vector<std::string> &&interfaces)
{
    auto bridge = conf_.FindInterfaceOfType<BridgeConfig>(bridge_name);
    if (!bridge)
    {
        return false;
    }

    std::unordered_set<InterfaceConfig *> interfaces_to_remove;
    interfaces_to_remove.reserve(interfaces.size());
    for (auto const &interface_name : interfaces)
    {
        auto interface = conf_.FindInterface(interface_name);
        if (!interface)
        {
            return false;
        }
        if (!bridge->interfaces_.contains(interface))
        {
            return false;
        }
        interfaces_to_remove.insert(interface);
    }

    for (auto &interface : interfaces_to_remove)
    {
        interface->RemoveFromBridge(*bridge);
    }

    if (bridge->interfaces_.size() < 2)
    {
        return RemoveInteface(bridge_name);
    }

    return true;
}

bool Configurator::UpdateVLANID(std::string const &vlan_interface_name, int new_vlan_id)
{
    auto vlan_interface = conf_.FindInterfaceOfType<VLANInterfaceConfig>(vlan_interface_name);
    if (!vlan_interface)
    {
        return false;
    }

    std::string new_vlan_interface_name = VLANInterfaceConfig::CreateVLANName(vlan_interface->parent_interface_->name_, new_vlan_id);
    if (!IsUniqueInterface(new_vlan_interface_name))
    {
        return false;
    }

    return vlan_interface->UpdateID(new_vlan_id);
}

bool Configurator::UpdateEthernetInterfaceSpeed(std::string const &ethernet_interface_name, int new_speed)
{
    auto ethernet_interface = conf_.FindInterfaceOfType<EthernetInterfaceConfig>(ethernet_interface_name);
    if (!ethernet_interface)
    {
        return false;
    }
    ethernet_interface->speed_ = new_speed;
    return true;
}
