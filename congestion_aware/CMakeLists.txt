# CMake Requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup project
project(AstraSim_Analytical_Congestion_Aware)

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
add_executable(AstraSim_Analytical_Congestion_Aware ${srcs})
target_sources(AstraSim_Analytical_Congestion_Aware PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/main.cc")

# Link libraries
target_link_libraries(AstraSim_Analytical_Congestion_Aware LINK_PUBLIC AstraSim)
target_link_libraries(AstraSim_Analytical_Congestion_Aware LINK_PUBLIC Boost::program_options)
target_link_libraries(AstraSim_Analytical_Congestion_Aware LINK_PUBLIC yaml-cpp)

# Properties
target_include_directories(AstraSim_Analytical_Congestion_Aware PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
set_target_properties(AstraSim_Analytical_Congestion_Aware
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../bin/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../lib/"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../lib/"
)
