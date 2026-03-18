cmake_minimum_required(VERSION 3.20)

project(router VERSION 0.1.0 LANGUAGES CXX)

add_executable(router
    src/public/Configurator.cpp
    src/private/BridgeConfig.cpp
    src/private/ConfiguratorData.cpp
    src/private/EthernetInterfaceConfig.cpp
    src/private/Interface.cpp
    src/private/IPAddr.cpp
    src/private/VLANInterfaceConfig.cpp
    src/main.cpp
)

# Enforce C23
set_property(TARGET router PROPERTY CXX_STANDARD 20)
set_property(TARGET router PROPERTY CXX_STANDARD_REQUIRED YES)
set_property(TARGET router PROPERTY CXX_EXTENSIONS NO)

# Make headers in src/ visible to files in test/
target_include_directories(router PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/private
    ${CMAKE_CURRENT_SOURCE_DIR}/src/public
)

# Warnings (portable set; GCC/Clang)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(router PRIVATE -Wall -Wextra -Wpedantic)
endif()


# Define DEBUG for Debug builds (and RelWithDebInfo, if you want)
target_compile_definitions(router PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:RelWithDebInfo>:DEBUG>
)

# Optional: choose optimization levels explicitly (CMake already sets sane defaults)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    target_compile_options(router PRIVATE
        $<$<CONFIG:Release>:-O2>
        $<$<CONFIG:MinSizeRel>:-Os>
        $<$<CONFIG:RelWithDebInfo>:-O2>
        $<$<CONFIG:Debug>:-O0>
    )
endif()
