#ifndef CONFIGURATOR_H__
#define CONFIGURATOR_H__

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "IPAddr.h"
#include "ConfiguratorData.h"

class Configurator
{
public:
    static Configurator &Get();

    bool AddEthernetInteface(std::string const &name, int speed, int MTU, bool fullduplex);
    bool BuildBridge(std::string const &name, std::vector<std::string> &&interfaces);
    bool AddVLAN(std::string const &parent_interface, int ID);
    bool RemoveInteface(std::string const &name);

    bool AddIPAddress(std::string const &interface_name, std::string const &ip_address);
    bool RemoveIPAddress(std::string const &interface_name, std::string const &ip_address);
    
    bool RenameInterface(std::string const &interface_name, std::string const &new_name);
    bool UpdateAdminState(std::string const &interface_name, bool up);
    bool UpdateBridgeSTP(std::string const &bridge_name, bool STP);
    bool UpdateBridgeAddInterfaces(std::string const &bridge_name, std::vector<std::string> &&interfaces);
    bool UpdateBridgeRemoveInterfaces(std::string const &bridge_name, std::vector<std::string> &&interfaces);
    // Deliberately not implementing UpdateVLANParentInterface as it would be too complex to implement 
    // correctly without possible side effects on failure, and is not a common use case. Instead, users 
    // can just remove and re-add the VLAN interface with the new parent interface.
    bool UpdateVLANID(std::string const &vlan_interface_name, int new_vlan_id);
    bool UpdateEthernetInterfaceSpeed(std::string const &ethernet_interface_name, int new_speed);
    bool UpdateEthernetInterfaceMTU(std::string const &ethernet_interface_name, int new_MTU);
    bool UpdateEthernetInterfaceDuplex(std::string const &ethernet_interface_name, bool fullduplex);

    ConfiguratorData &GetData() { return conf_; }

private:
    bool IsUniqueInterface(std::string const &name) const;
    ConfiguratorData conf_;
};

#endif // CONFIGURATOR_H__
