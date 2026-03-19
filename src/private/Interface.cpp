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
        configurator_.used_ip_addresses_.erase(ip_address.first);
    }

    return true;
}

bool InterfaceConfig::AddIPAddress(std::string const &ip_address, uint8_t prefix_length)
{
    uint8_t max_prefix_length = 0;
    if (!IPAddress::IsValid(ip_address, max_prefix_length))
    {
        return false;
    }
    if (prefix_length > max_prefix_length || prefix_length == 0)
    {
        return false;
    }
    if (configurator_.used_ip_addresses_.contains(ip_address))
    {
        return false;
    }

    configurator_.used_ip_addresses_.insert(ip_address);
    assigned_ip_addresses_.insert({ip_address, prefix_length});
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
