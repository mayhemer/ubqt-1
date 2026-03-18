# Technical Assignment

**Time budget:** 2 -- 3 hours (take-home)

**Deadline:** 1 week from receiving this assignment

**You may use AI-assisted tools.** We evaluate the quality and coherence
of the final design, not your ability to type code from memory.

## Problem

Imagine a Linux-based gateway/router that runs a daemon managing network
interface configuration. The daemon keeps an in-memory representation of
the desired state and applies it to the system via netlink/ioctl.

Your task is to design the **in-memory configuration model** -- the C++
types, classes, and API that the daemon would use internally. You do **not**
need to implement the daemon itself, the system backend (netlink/ioctl), or
JSON parsing.

## Requirements

The API must support the following interface types:

| Type | Key properties |
|------|----------------|
| **Ethernet** (physical) | speed, duplex, MTU |
| **VLAN** | parent interface, VLAN ID (1--4094) |
| **Bridge** | member interfaces, STP on/off |

Each interface may have:
- Zero or more IPv4/IPv6 addresses (with prefix length).
- An admin state (up / down).

The API must support adding, updating, and removing interfaces.

## Constraints to model

- A VLAN's parent must be a physical Ethernet interface (not another VLAN,
  not a bridge).
- Bridge members must not have IP addresses assigned (traffic is bridged at
  L2; IP addresses belong on the bridge interface itself).
- An interface referenced by others (e.g. an Ethernet port used as a VLAN
  parent) must not be removable until all dependents are removed first.
- All constraints must be enforced at the point of mutation, not as a
  separate validation pass. The configuration must always be in a valid
  state.


## Deliverables

The solution should be delivered as a **pull request on a public git
repository** (GitHub, GitLab, etc.).

1. **Public header file(s)** defining the API. Must compile with C++20.
2. **Implementation** of the configuration container. As a rule of thumb,
   if this is significantly longer than the header, the design may be
   overly complex.
3. **Example usage** (`main.cpp`) showing a realistic scenario: building a
   configuration, updating it, and tearing it down.
4. **CMakeLists.txt** to build the project.
5. **README** with:
   - Build instructions.
   - Brief description of the design -- key decisions and what alternatives
     you considered.
   - How you would add a new interface type (e.g. Bond/LAG).

## What we do NOT expect

- Working netlink/ioctl or JSON code.
- Unit tests (though we appreciate them if time allows).
