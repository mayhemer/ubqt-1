#include "ConfiguratorData.h"

#include "BridgeConfig.h"
#include "EthernetInterfaceConfig.h"
#include "VLANInterfaceConfig.h"

InterfaceConfig *ConfiguratorData::FindInterface(std::string const &name) const
{
    auto interface_it = interfaces_.find(name);
    return interface_it == interfaces_.end() ? nullptr : interface_it->second.get();
}
