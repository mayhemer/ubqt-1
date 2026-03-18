#include "ConfiguratorData.h"

#include "BridgeConfig.h"
#include "EthernetInterfaceConfig.h"
#include "VLANInterfaceConfig.h"

InterfaceConfig *ConfiguratorData::FindInterface(std::string const &name) const
{
    auto interface_it = interfaces_.find(name);
    return interface_it == interfaces_.end() ? nullptr : interface_it->second.get();
}

EthernetInterfaceConfig *ConfiguratorData::FindEthernetInterface(std::string const &name) const
{
    auto interface = FindInterface(name);
    return interface && interface->GetType() == InterfaceType::Ethernet ? static_cast<EthernetInterfaceConfig *>(interface) : nullptr;
}

BridgeConfig *ConfiguratorData::FindBridge(std::string const &name) const
{
    auto interface = FindInterface(name);
    return interface && interface->GetType() == InterfaceType::Bridge ? static_cast<BridgeConfig *>(interface) : nullptr;
}

VLANInterfaceConfig *ConfiguratorData::FindVLANInterface(std::string const &name) const
{
    auto interface = FindInterface(name);
    return interface && interface->GetType() == InterfaceType::VLAN ? static_cast<VLANInterfaceConfig *>(interface) : nullptr;
}
