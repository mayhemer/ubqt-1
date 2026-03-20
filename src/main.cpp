#include "Configurator.h"

#include <cassert>

int main()
{
    auto &configurator = Configurator::Get();

    bool result;

    result = configurator.AddEthernetInteface("eth0", 1000, 1500, EthernetInterfaceConfig::DuplexMode::AutoNeg);
    assert(result);
    result = configurator.AddEthernetInteface("eth1", 1000, 1500, EthernetInterfaceConfig::DuplexMode::AutoNeg);
    assert(result);
    result = configurator.AddEthernetInteface("eth1", 1000, 1500, EthernetInterfaceConfig::DuplexMode::AutoNeg);
    // Already exists
    assert(!result);

    result = configurator.AddIPAddress("eth0", "192.168.1.1", 24);
    assert(result);

    result = configurator.BuildBridge("br0", {"eth0", "eth1"});
    // First remove the IP address from eth0, otherwise it can't be added to the bridge
    assert(!result);

    result = configurator.RemoveIPAddress("eth0", "192.168.1.1");
    assert(result);

    result = configurator.BuildBridge("br0", {"eth0", "eth1"});
    assert(result);

    result = configurator.AddVLAN("eth0", 10);
    assert(result);
    result = configurator.AddVLAN("eth1", 20);
    assert(result);
    result = configurator.AddVLAN("eth1", 20);
    // Already exists
    assert(!result);

    result = configurator.AddVLAN("br0", 100);
    // Can't add VLAN on a bridge interface
    assert(!result);

    result = configurator.AddVLAN("eth0.10", 123);
    // Can't add VLAN on a VLAN interface
    assert(!result);

    result = configurator.AddIPAddress("br0", "192.168.1.1", 24);
    assert(result);
    result = configurator.AddIPAddress("eth0.10", "192.168.1.2", 24);
    assert(result);
    result = configurator.AddIPAddress("eth1.20", "192.168.1.3", 24);
    assert(result);
    result = configurator.AddIPAddress("eth1.20", "192.168.1.3", 24);
    // Already used
    assert(!result);

    result = configurator.RemoveIPAddress("eth0.10", "192.168.1.2");
    assert(result);
    result = configurator.RemoveIPAddress("eth0.10", "192.168.1.2");
    // Already removed
    assert(!result);
    result = configurator.RemoveIPAddress("eth0.10", "192.168.1.244");
    // Not assigned
    assert(!result);

    result = configurator.AddIPAddress("eth0", "192.168.1.4", 24);
    // Can't add IP address to an interface that is in a bridge
    assert(!result);

    assert(configurator.GetData().interfaces_.size() == 5);
    assert(configurator.GetData().used_ip_addresses_.size() == 2);

    result = configurator.RemoveInteface("eth0");
    // It's in the bridge and keeps a VLAN interface alive
    assert(!result);
    result = configurator.RemoveInteface("eth1");
    // It's in the bridge and keeps a VLAN interface alive
    assert(!result);

    result = configurator.RemoveInteface("br0");
    assert(result);

    result = configurator.RemoveInteface("eth0");
    // It keeps a VLAN interface alive
    assert(!result);
    result = configurator.RemoveInteface("eth1");
    // It keeps a VLAN interface alive
    assert(!result);

    result = configurator.BuildBridge("br0", {"eth0", "eth1"});
    assert(result);

    result = configurator.RemoveInteface("eth0");
    // It's in the bridge and keeps a VLAN interface alive
    assert(!result);
    result = configurator.RemoveInteface("eth1");
    // It's in the bridge and keeps a VLAN interface alive
    assert(!result);

    result = configurator.UpdateVLANID("eth1.20", 21);
    assert(result);

    result = configurator.RemoveInteface("br0");
    assert(result);
    result = configurator.RemoveInteface("eth0.10");
    assert(result);
    result = configurator.RemoveInteface("eth0");
    assert(result);
    result = configurator.RemoveInteface("eth1");
    // Keeps a VLAN interface alive
    assert(!result);

    result = configurator.RemoveInteface("eth1.20");
    // eth1.20 was renamed to eth1.21
    assert(!result);
    result = configurator.RemoveInteface("eth1.21");
    assert(result);
    result = configurator.RemoveInteface("eth1");
    assert(result);

    result = configurator.AddVLAN("eth0", 10);
    // Parent interface doesn't exist
    assert(!result);

    result = configurator.BuildBridge("br0", {"eth0", "eth1"});
    // eth0/1 don't exist
    assert(!result);

    assert(configurator.GetData().interfaces_.empty());
    assert(configurator.GetData().used_ip_addresses_.empty());

    result = configurator.AddEthernetInteface("eth2", 1000, 1500, EthernetInterfaceConfig::DuplexMode::Full);
    assert(result);
    result = configurator.AddEthernetInteface("eth3", 1000, 1500, EthernetInterfaceConfig::DuplexMode::Full);
    assert(result);
    result = configurator.AddEthernetInteface("eth4", 1000, 1500, EthernetInterfaceConfig::DuplexMode::Full);
    assert(result);

    result = configurator.BuildBridge("br0", {"eth2", "eth3"});
    assert(result);
    result = configurator.AddIPAddress("br0", "10.0.0.123", 8);
    assert(result);
    result = configurator.UpdateBridgeAddInterfaces("br0", {"eth4"});
    assert(result);
    result = configurator.UpdateBridgeRemoveInterfaces("br0", {"eth2"});
    assert(result);
    result = configurator.UpdateBridgeRemoveInterfaces("br0", {"eth3"});
    assert(result);

    result = configurator.RemoveInteface("br0");
    // Bridge was torn by dropping interface count to less than 2
    assert(!result);

    assert(configurator.GetData().interfaces_.size() == 3);
    assert(configurator.GetData().used_ip_addresses_.empty());

    return 0;
}
