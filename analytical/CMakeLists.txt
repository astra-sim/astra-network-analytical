# Setup project
project (AnalyticalAstra)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/local/include")
link_directories(/usr/local/lib/)

# Package requirement
find_package(Boost 1.40 REQUIRED COMPONENTS program_options)

# Include src files to compile
file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/src/*.hh"
        "${PROJECT_SOURCE_DIR}/src/*.cc"
        )

# Compile sources
add_executable(AnalyticalAstra ${srcs})

# Link libraries
target_link_libraries(AnalyticalAstra LINK_PUBLIC AstraSim)
target_link_libraries(AnalyticalAstra LINK_PRIVATE Boost::program_options -lprotobuf -lpthread -lz)
include_directories(
  "${PROJECT_SOURCE_DIR}/src"
  "${PROJECT_SOURCE_DIR}/../../../graph_frontend/chakra/")

# Resulting binary location settings
set_target_properties(AnalyticalAstra
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "bin/"
        LIBRARY_OUTPUT_DIRECTORY "lib/"
        ARCHIVE_OUTPUT_DIRECTORY "lib/"
        )
