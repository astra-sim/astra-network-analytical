# CMake Requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup project
project(AstraSim_Analytical_Congestion_Unaware)

# Find required packages
find_package(Boost REQUIRED COMPONENTS program_options)

# Include src files to compile
file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/api/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/api/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/event-queue/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/event-queue/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/helper/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/helper/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/topology/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/topology/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../../astra-sim/network_frontend/analytical/congestion_unaware/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../statistics/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../statistics/*.cc"
)

# Compile Congestion Unaware Backend
add_executable(AstraSim_Analytical_Congestion_Unaware ${srcs})
target_sources(AstraSim_Analytical_Congestion_Unaware PRIVATE "${PROJECT_SOURCE_DIR}/main.cc")

# Link libraries
target_link_libraries(AstraSim_Analytical_Congestion_Unaware LINK_PUBLIC AstraSim)
target_link_libraries(AstraSim_Analytical_Congestion_Unaware LINK_PUBLIC Boost::program_options)

# Properties
target_include_directories(AstraSim_Analytical_Congestion_Unaware PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(AstraSim_Analytical_Congestion_Unaware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../astra-sim/network_frontend/analytical/congestion_unaware)
set_target_properties(AstraSim_Analytical_Congestion_Unaware
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../bin/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../lib/"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../lib/"
)
