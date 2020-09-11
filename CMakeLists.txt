cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(AnalyticalBackend)

file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/src/*.hh"
        "${PROJECT_SOURCE_DIR}/src/*.cc"
        "${PROJECT_SOURCE_DIR}/astra-sim/system/*.hh"
        "${PROJECT_SOURCE_DIR}/astra-sim/system/*.cc"
        "${PROJECT_SOURCE_DIR}/astra-sim/workload/*.hh"
        "${PROJECT_SOURCE_DIR}/astra-sim/workload/*.cc"
        )

add_executable(AnalyticalBackend ${srcs})

set_target_properties(AnalyticalBackend
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "../lib"
        LIBRARY_OUTPUT_DIRECTORY "../lib"
        RUNTIME_OUTPUT_DIRECTORY "../bin"
        )
