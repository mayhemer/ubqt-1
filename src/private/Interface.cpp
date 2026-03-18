#include "Interface.h"

#include "ConfiguratorData.h"

InterfaceConfig::InterfaceConfig(std::string const &name, ConfiguratorData &configurator)
    : configurator_(configurator), name_(name)
{
}

bool InterfaceConfig::SetAdminState(bool up)
{
    is_up_ = up;
    return true;
}

bool InterfaceConfig::Rename(std::string const &new_name)
{
    auto self = configurator_.interfaces_.extract(name_);
    name_ = new_name;

    self.key() = name_;
    configurator_.interfaces_.insert(std::move(self));
    return true;
}

bool InterfaceConfig::Remove()
{
    for (auto const &ip_address : assigned_ip_addresses_)
    {
        configurator_.used_ip_addresses_.erase(ip_address);
    }

    return true;
}

bool InterfaceConfig::AddIPAddress(std::string const &ip_address)
{
    if (!IPAddress::IsValid(ip_address))
    {
        return false;
    }
    auto existing_ip = configurator_.used_ip_addresses_.find(ip_address);
    if (existing_ip != configurator_.used_ip_addresses_.end())
    {
        return false;
    }

    configurator_.used_ip_addresses_.insert(ip_address);
    assigned_ip_addresses_.insert(ip_address);
    return true;
}

bool InterfaceConfig::RemoveIPAddress(std::string const &ip_address)
{
    auto existing_ip = assigned_ip_addresses_.find(ip_address);
    if (existing_ip == assigned_ip_addresses_.end())
    {
        return false;
    }

    configurator_.used_ip_addresses_.erase(ip_address);
    assigned_ip_addresses_.erase(existing_ip);
    return true;
}
