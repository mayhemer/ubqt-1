#ifndef CONFIGURATOR_DATA_H__
#define CONFIGURATOR_DATA_H__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "IPAddr.h"
#include "Interface.h"

struct ConfiguratorData
{
    InterfaceConfig *FindInterface(std::string const &name) const;

    template <typename T>
    T *FindInterfaceOfType(std::string const &name) const
    {
        auto interface = FindInterface(name);
        return interface && interface->interface_type_ == T::InterfaceTypeValue ? static_cast<T *>(interface) : nullptr;
    }

    std::unordered_map<std::string, std::unique_ptr<InterfaceConfig>> interfaces_;
    UniqueIPAddresses used_ip_addresses_;
};

#endif // CONFIGURATOR_DATA_H__
