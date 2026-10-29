# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2026 Advanced Micro Devices, Inc.

# CMake Requirement
cmake_minimum_required(VERSION 3.19)

# C++ requirement
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Setup project
project(Analytical)

# Compilation target
set(BUILDTARGET "all" CACHE STRING "Compilation target ([all]/congestion_unaware/congestion_aware)")

# Can be compiled into either library or executable
option(NETWORK_BACKEND_BUILD_AS_LIBRARY "Build as a library" OFF)

# Compile external libraries
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(FetchYamlCpp)

# Source files to compile
set(srcs_common
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/BaseTopology.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/NetworkFunction.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/TicketModel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/event-queue/Event.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/event-queue/EventList.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/event-queue/EventQueue.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/common/network-parser/NetworkParser.cpp
)

set(srcs_congestion_unaware
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/basic-topology/BasicTopology.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/basic-topology/CustomTopology.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/basic-topology/FullyConnected.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/basic-topology/Ring.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/basic-topology/Switch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/multi-dim-topology/MultiDimTopology.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/topology/Helper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/topology/Topology.cpp
)

set(srcs_congestion_aware
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/basic-topology/BasicTopology.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/basic-topology/CustomTopology.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/basic-topology/FullyConnected.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/basic-topology/Ring.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/basic-topology/Switch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/network/Chunk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/network/Device.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/network/Link.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/topology/Helper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/topology/Topology.cpp
)

# Header files, listed for IDE visibility only (skipped at compile time by extension)
set(hdrs_common
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/common/BaseTopology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/common/Event.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/common/EventList.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/common/EventQueue.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/common/NetworkFunction.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/common/NetworkParser.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/common/TicketModel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/common/Type.h
)

set(hdrs_congestion_unaware
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_unaware/BasicTopology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_unaware/CustomTopology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_unaware/FullyConnected.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_unaware/Helper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_unaware/MultiDimTopology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_unaware/Ring.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_unaware/Switch.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_unaware/Topology.h
)

set(hdrs_congestion_aware
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/BasicTopology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/Chunk.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/CustomTopology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/Device.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/FullyConnected.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/Helper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/Link.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/Ring.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/Switch.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/Topology.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/astra-network-analytical/congestion_aware/Type.h
)

# Compile Congestion Unaware Backend
if (BUILDTARGET STREQUAL "all" OR BUILDTARGET STREQUAL "congestion_unaware")
    if (NETWORK_BACKEND_BUILD_AS_LIBRARY)
        add_library(Analytical_Congestion_Unaware STATIC ${srcs_congestion_unaware} ${srcs_common})

        # Properties
        set_target_properties(Analytical_Congestion_Unaware
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin/
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
        )
    else ()
        add_executable(Analytical_Congestion_Unaware ${srcs_congestion_unaware} ${srcs_common})
        target_sources(Analytical_Congestion_Unaware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_unaware/example.cpp)

        # Directory of the sample network configurations used by example.cpp
        target_compile_definitions(Analytical_Congestion_Unaware PRIVATE EXAMPLE_INPUTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/input")

        # Properties
        set_target_properties(Analytical_Congestion_Unaware
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/
        )
    endif ()

    # Common properties
    set_target_properties(Analytical_Congestion_Unaware PROPERTIES COMPILE_WARNING_AS_ERROR ON)

    # Headers, listed for IDE visibility only
    target_sources(Analytical_Congestion_Unaware PRIVATE ${hdrs_congestion_unaware} ${hdrs_common})

    # Link libraries
    target_link_libraries(Analytical_Congestion_Unaware PUBLIC yaml-cpp::yaml-cpp)

    # Include directories
    target_include_directories(Analytical_Congestion_Unaware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
endif ()

# Compile Congestion Aware Backend
if (BUILDTARGET STREQUAL "all" OR BUILDTARGET STREQUAL "congestion_aware")
    if (NETWORK_BACKEND_BUILD_AS_LIBRARY)
        add_library(Analytical_Congestion_Aware STATIC ${srcs_congestion_aware} ${srcs_common})

        # Properties
        set_target_properties(Analytical_Congestion_Aware
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin/
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
        )
    else ()
        add_executable(Analytical_Congestion_Aware ${srcs_congestion_aware} ${srcs_common})
        target_sources(Analytical_Congestion_Aware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/congestion_aware/example.cpp)

        # Directory of the sample network configurations used by example.cpp
        target_compile_definitions(Analytical_Congestion_Aware PRIVATE EXAMPLE_INPUTS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/input")

        # Properties
        set_target_properties(Analytical_Congestion_Aware
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/
        )
    endif ()

    # Common properties
    set_target_properties(Analytical_Congestion_Aware PROPERTIES COMPILE_WARNING_AS_ERROR ON)

    # Headers, listed for IDE visibility only
    target_sources(Analytical_Congestion_Aware PRIVATE ${hdrs_congestion_aware} ${hdrs_common})

    # Link libraries
    target_link_libraries(Analytical_Congestion_Aware PUBLIC yaml-cpp::yaml-cpp)

    # Include directories
    target_include_directories(Analytical_Congestion_Aware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
endif ()
