# Build instructions

You need cmake (3.20+), ninja (1.13+), compilation tested against GCC 15.2 and AppleCLang 17 (on macOS).

```bash
# From the repository root, assuming your compiler of choise is in your PATH, run:
# ( If not, add -DCMAKE_CXX_COMPILER:FILEPATH={path-to-compiler} )
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S . -B ./build -G Ninja
cmake --build ./build --config Debug --target all
# To run the example:
./build/router
```

# Design

I've chosen to go with defining a base class for a general declarion of an interface common properties and virtualization of all possible functions for PI manipulation, adding to a bridge and adding VLANs.  This design naturally, via virtualization, enables or disables certain operations according the interface type and can be easily modified when we want e.g. to allow VLAN of off a bridge.  It greatly simplifies the public API and doesn't add a need to do the complicated type/state checking inside the configuration API implementation itself.

The main configurator is a static singleton (for simplicity).  It holds a map of interface names to interface configurations, via a unique pointer reference.  This naturally ensures interfaces unique naming.  Child/parent references are using raw pointers to allow simple interface renaming - if this was not required I'd prefer to keep references by a name instead.  It is safe to use raw pointers as it's ensured 1) by the logic that parents can't be removed when referenced 2) by hiding the objects from public consumers - no external manipulation allowed.  When an interface is removed from the map, it is released from memory.

- The public API part is in the `src/public` directory.
- An example usage in `src/main.cpp`.
- Containers implementation in `src/private`.

Specific classes are implemented for the three desired types: ethernet, VLAN and a bridge.  As the requirement was to be able to update existing interface properties, some type of RTTI was implemented, using enumeration and Find{Type}Interface functions on the configurator, using type compare and static_cast'ing.

# Adding new interface type

For instance, a LAG interface representation is very similar to the bridge pattern.  I would add new methods on the base interface for creating a LAG interface with similar functionality, possibly even reuse some of it (parent/child relation etc.) possibly by defining some kind of an 'AggregateInterface' specialized abstract class as a base for bridge and LAG interfaces properties.
