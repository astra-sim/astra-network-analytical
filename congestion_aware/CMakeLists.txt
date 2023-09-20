# CMake Requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup project
project(Analytical_Congestion_Aware)

# Can be compiled into either library or executable
option(NETWORK_BACKEND_BUILD_AS_LIBRARY "Build as a library" OFF)

# Find required packages
find_package(Boost REQUIRED COMPONENTS program_options)

# Compile external libraries
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/yaml-cpp")

# Include src files to compile
file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/api/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/api/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/event-queue/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/event-queue/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/helper/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/helper/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/network/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/network/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/topology/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/topology/*.cc"
)

# Compile Congestion Aware Backend
if (NETWORK_BACKEND_BUILD_AS_LIBRARY)
    add_library(Analytical_Congestion_Aware STATIC ${srcs})
else ()
    add_executable(Analytical_Congestion_Aware ${srcs})
    target_sources(Analytical_Congestion_Aware PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/main.cc")
endif ()

# Link libraries
target_link_libraries(Analytical_Congestion_Aware LINK_PUBLIC Boost::program_options)
target_link_libraries(Analytical_Congestion_Aware LINK_PUBLIC yaml-cpp)

# Properties
target_include_directories(Analytical_Congestion_Aware PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(Analytical_Congestion_Aware
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../bin/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../lib/"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../lib/"
)
