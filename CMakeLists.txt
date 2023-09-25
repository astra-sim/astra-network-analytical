# CMake Requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup project
project(Analytical)

# Compilation target
set(BUILDTARGET "all" CACHE STRING "Compilation target ([all]/congestion_unaware/congestion_aware)")

# Can be compiled into either library or executable
option(NETWORK_BACKEND_BUILD_AS_LIBRARY "Build as a library" OFF)

# Find required packages
find_package(Boost REQUIRED COMPONENTS program_options)

# Compile external libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/yaml-cpp yaml-cpp)

# Include src files to compile
file(GLOB common_srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/common/event-queue/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/common/network-parser/*.cc
)

file(GLOB congestion_unaware_srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_unaware/topology/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_unaware/basic-topology/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_unaware/multi-dim-topology/*.cc
)

file(GLOB congestion_aware_srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/network/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/topology/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/basic-topology/*.cc
)

# Compile Congestion Unaware Backend
if (BUILDTARGET STREQUAL "all" OR BUILDTARGET STREQUAL "congestion_unaware")
    if (NETWORK_BACKEND_BUILD_AS_LIBRARY)
        add_library(Analytical_Congestion_Unaware STATIC ${congestion_unaware_srcs} ${common_srcs})

        # Properties
        set_target_properties(Analytical_Congestion_Unaware
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin/
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
        )
    else ()
        add_executable(Analytical_Congestion_Unaware ${congestion_unaware_srcs} ${common_srcs})
        target_sources(Analytical_Congestion_Unaware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/congestion_unaware/example.cc)

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

    # Link libraries
    target_link_libraries(Analytical_Congestion_Unaware LINK_PUBLIC Boost::program_options)
    target_link_libraries(Analytical_Congestion_Unaware LINK_PUBLIC yaml-cpp)

    # Include directories
    target_include_directories(Analytical_Congestion_Unaware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(Analytical_Congestion_Unaware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../)
    target_include_directories(Analytical_Congestion_Unaware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/)
endif ()

# Compile Congestion Aware Backend
if (BUILDTARGET STREQUAL "all" OR BUILDTARGET STREQUAL "congestion_aware")
    if (NETWORK_BACKEND_BUILD_AS_LIBRARY)
        add_library(Analytical_Congestion_Aware STATIC ${congestion_aware_srcs} ${common_srcs})

        # Properties
        set_target_properties(Analytical_Congestion_Aware
                PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin/
                LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
                ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
        )
    else ()
        add_executable(Analytical_Congestion_Aware ${congestion_aware_srcs} ${common_srcs})
        target_sources(Analytical_Congestion_Aware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/example.cc)

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

    # Link libraries
    target_link_libraries(Analytical_Congestion_Aware LINK_PUBLIC Boost::program_options)
    target_link_libraries(Analytical_Congestion_Aware LINK_PUBLIC yaml-cpp)

    # Include directories
    target_include_directories(Analytical_Congestion_Aware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(Analytical_Congestion_Aware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../)
    target_include_directories(Analytical_Congestion_Aware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/)
endif ()
