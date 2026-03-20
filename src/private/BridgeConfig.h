#ifndef BRIDGE_CONFIG_H__
#define BRIDGE_CONFIG_H__

#include "Interface.h"

#include <unordered_map>

struct BridgeConfig : public InterfaceConfig
{
    inline static constexpr InterfaceType InterfaceTypeValue = InterfaceType::Bridge;

    BridgeConfig() = delete;
    BridgeConfig(std::string const &name, ConfiguratorData &configurator, bool STP);

    virtual bool Remove() override;

    std::unordered_set<InterfaceConfig *> interfaces_;
    bool STP_{false};
};

#endif
