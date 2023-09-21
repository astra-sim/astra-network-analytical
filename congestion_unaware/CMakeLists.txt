# CMake Requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup project
project(Analytical_Congestion_Unaware)

# Can be compiled into either library or executable
option(NETWORK_BACKEND_BUILD_AS_LIBRARY "Build as a library" OFF)

# Find required packages
find_package(Boost REQUIRED COMPONENTS program_options)

# Include src files to compile
file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/event-queue/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/event-queue/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/helper/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/helper/*.cc"
        "${CMAKE_CURRENT_SOURCE_DIR}/topology/*.hh"
        "${CMAKE_CURRENT_SOURCE_DIR}/topology/*.cc"
)

# Compile Congestion Unaware Backend
if (NETWORK_BACKEND_BUILD_AS_LIBRARY)
    add_library(Analytical_Congestion_Unaware STATIC ${srcs})
else ()
    add_executable(Analytical_Congestion_Unaware ${srcs})
    target_sources(Analytical_Congestion_Unaware PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/main.cc")
endif ()

# Link libraries
target_link_libraries(Analytical_Congestion_Unaware LINK_PUBLIC Boost::program_options)

# Properties
target_include_directories(Analytical_Congestion_Unaware PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(Analytical_Congestion_Unaware PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../../../")
set_target_properties(Analytical_Congestion_Unaware
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../bin/"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../lib/"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../lib/"
)
