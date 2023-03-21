# CMake requirement
cmake_minimum_required(VERSION 3.15)

# C++ requirement
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler requirement
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.3)
        message(FATAL_ERROR "g++ (GNU) version should be greater than 5.3, but found ${CMAKE_CXX_COMPILER_VERSION}")
    endif ()
endif ()

# Setup project
project(AstraCongestion)

# Package requirement
find_package(Boost REQUIRED COMPONENTS program_options)

# Compile external libraries
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/yaml-cpp")

# Include src files to compile
file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/api/*.hh"
        "${PROJECT_SOURCE_DIR}/api/*.cc"
        "${PROJECT_SOURCE_DIR}/event-queue/*.hh"
        "${PROJECT_SOURCE_DIR}/event-queue/*.cc"
        "${PROJECT_SOURCE_DIR}/helper/*.hh"
        "${PROJECT_SOURCE_DIR}/helper/*.cc"
        "${PROJECT_SOURCE_DIR}/network/*.hh"
        "${PROJECT_SOURCE_DIR}/network/*.cc"
        "${PROJECT_SOURCE_DIR}/topology/*.hh"
        "${PROJECT_SOURCE_DIR}/topology/*.cc"
        )

# Compile sources
add_executable(AstraCongestion ${srcs})
target_sources(AstraCongestion PRIVATE "${PROJECT_SOURCE_DIR}/main.cc")

# Link libraries
target_link_libraries(AstraCongestion LINK_PUBLIC AstraSim)
target_link_libraries(AstraCongestion LINK_PRIVATE Boost::program_options yaml-cpp)
include_directories("${PROJECT_SOURCE_DIR}")

# Resulting binary location settings
set_target_properties(AstraCongestion
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "bin/"
        LIBRARY_OUTPUT_DIRECTORY "lib/"
        ARCHIVE_OUTPUT_DIRECTORY "lib/"
        )
