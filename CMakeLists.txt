cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(AnalyticalBackend)

add_executable(AnalyticalBackend
        main.cc
        Event.hh Event.cc
        EventQueueEntry.hh EventQueueEntry.cc
        EventQueue.hh EventQueue.cc)

set_target_properties(AnalyticalBackend
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "lib"
        LIBRARY_OUTPUT_DIRECTORY "lib"
        RUNTIME_OUTPUT_DIRECTORY "bin"
        )
