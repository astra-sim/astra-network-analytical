# CMake Requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement: C++14 is required
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Project setup
project(AnalyticalBackend)

# Boost package is required
find_package(Boost 1.40 REQUIRED COMPONENTS program_options)

# Minimum compiler version requirements
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
        message(FATAL_ERROR "Insufficient gcc version")
    endif()
endif()

# Include src files to compile
file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/src/*.hh"
        "${PROJECT_SOURCE_DIR}/src/*.cc"
        "${PROJECT_SOURCE_DIR}/astra-sim/system/*.hh"
        "${PROJECT_SOURCE_DIR}/astra-sim/system/*.cc"
        "${PROJECT_SOURCE_DIR}/astra-sim/workload/*.hh"
        "${PROJECT_SOURCE_DIR}/astra-sim/workload/*.cc"
        )

# Compile sources
add_executable(AnalyticalBackend ${srcs})

# Link Boost library after compilation
target_link_libraries(AnalyticalBackend Boost::program_options)

# Save resulting binary in the bin/ directory.
set_target_properties(AnalyticalBackend
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "../bin"
        )
