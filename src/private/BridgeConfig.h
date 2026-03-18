#ifndef BRIDGE_CONFIG_H__
#define BRIDGE_CONFIG_H__

#include "Interface.h"

#include <unordered_map>

struct BridgeConfig : public InterfaceConfig
{
    BridgeConfig() = delete;
    BridgeConfig(std::string const &name, ConfiguratorData &configurator, bool STP);

    virtual InterfaceType GetType() const override { return InterfaceType::Bridge; }
    
    virtual bool Remove() override;

    std::unordered_set<InterfaceConfig *> interfaces_;
    bool STP_{false};
};

#endif
