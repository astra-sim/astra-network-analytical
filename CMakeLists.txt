cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(AnalyticalBackend)

find_package(Boost 1.40 REQUIRED COMPONENTS program_options)

file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/src/*.hh"
        "${PROJECT_SOURCE_DIR}/src/*.cc"
        "${PROJECT_SOURCE_DIR}/astra-sim/system/*.hh"
        "${PROJECT_SOURCE_DIR}/astra-sim/system/*.cc"
        "${PROJECT_SOURCE_DIR}/astra-sim/workload/*.hh"
        "${PROJECT_SOURCE_DIR}/astra-sim/workload/*.cc"
        )

add_executable(AnalyticalBackend ${srcs})

target_link_libraries(AnalyticalBackend Boost::program_options)
set_target_properties(AnalyticalBackend
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "../lib"
        LIBRARY_OUTPUT_DIRECTORY "../lib"
        RUNTIME_OUTPUT_DIRECTORY "../bin"
        )
