cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(AnalyticalBackend)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/astra-sim/*.hh"
        "${PROJECT_SOURCE_DIR}/astra-sim/*.cc"
        )

add_executable(AnalyticalBackend
        ${srcs}
        )

set_target_properties(AnalyticalBackend
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "lib"
        LIBRARY_OUTPUT_DIRECTORY "lib"
        RUNTIME_OUTPUT_DIRECTORY "bin"
        )
