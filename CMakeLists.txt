# CMake Requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement: C++14 is required
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Project setup
project(AnalyticalBackend)

# Directories for packages
link_directories(${PROJECT_SOURCE_DIR}/lib)  # libAstraSim
find_package(Boost 1.40 REQUIRED COMPONENTS program_options)

# Minimum compiler version requirements
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
        message(FATAL_ERROR "g++ (GNU) version should be greater than 5.3, but found ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
endif()

# Include src files to compile
file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/src/*.hh"
        "${PROJECT_SOURCE_DIR}/src/*.cc"
        )

# Compile sources
add_executable(AnalyticalBackend ${srcs})

# Link libraries
target_link_libraries(AnalyticalBackend LINK_PUBLIC AstraSim)
target_link_libraries(AnalyticalBackend LINK_PRIVATE Boost::program_options)

# Save resulting binary in the bin/ directory.
set_target_properties(AnalyticalBackend
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "../bin"
        )
