#include "BridgeConfig.h"

#include <cassert>

#include "ConfiguratorData.h"

BridgeConfig::BridgeConfig(std::string const &name, ConfiguratorData &configurator, bool STP)
    : InterfaceConfig(name, configurator), STP_(STP)
{
}

bool BridgeConfig::Remove()
{
    auto interfaces_local = interfaces_;
    for (auto interface : interfaces_local)
    {
        interface->RemoveFromBridge(*this);
    }

    assert(interfaces_.empty());
    return InterfaceConfig::Remove();
}
