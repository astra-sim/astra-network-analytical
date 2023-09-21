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
file(GLOB congestion_unaware_srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_unaware/event-queue/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_unaware/helper/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_unaware/topology/*.cc
)

file(GLOB congestion_aware_srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/event-queue/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/helper/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/network/*.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/topology/*.cc
)

# Compile Congestion Unaware Backend
if (BUILDTARGET STREQUAL "all" OR BUILDTARGET STREQUAL "congestion_unaware")
    if (NETWORK_BACKEND_BUILD_AS_LIBRARY)
        add_library(Analytical_Congestion_Unaware STATIC ${congestion_unaware_srcs})
    else ()
        add_executable(Analytical_Congestion_Unaware ${congestion_unaware_srcs})
        target_sources(Analytical_Congestion_Unaware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/congestion_unaware/main.cc)
    endif ()

    # Link libraries
    target_link_libraries(Analytical_Congestion_Unaware LINK_PUBLIC Boost::program_options)
    target_link_libraries(Analytical_Congestion_Unaware LINK_PUBLIC yaml-cpp)

    # Include directories
    target_include_directories(Analytical_Congestion_Unaware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(Analytical_Congestion_Unaware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../)  # TODO: remove this
    target_include_directories(Analytical_Congestion_Unaware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/)

    # Properties
    set_target_properties(Analytical_Congestion_Unaware
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin/
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
    )
endif ()

# Compile Congestion Aware Backend
if (BUILDTARGET STREQUAL "all" OR BUILDTARGET STREQUAL "congestion_aware")
    if (NETWORK_BACKEND_BUILD_AS_LIBRARY)
        add_library(Analytical_Congestion_Aware STATIC ${congestion_aware_srcs})
    else ()
        add_executable(Analytical_Congestion_Aware ${congestion_aware_srcs})
        target_sources(Analytical_Congestion_Aware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/congestion_aware/main.cc)
    endif ()

    # Link libraries
    target_link_libraries(Analytical_Congestion_Aware LINK_PUBLIC Boost::program_options)
    target_link_libraries(Analytical_Congestion_Aware LINK_PUBLIC yaml-cpp)

    # Include directories
    target_include_directories(Analytical_Congestion_Aware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_include_directories(Analytical_Congestion_Aware PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/extern/)

    # Properties
    set_target_properties(Analytical_Congestion_Aware
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../bin/
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../lib/
    )
endif ()
