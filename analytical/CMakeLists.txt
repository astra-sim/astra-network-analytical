# Setup project
project (AnalyticalAstra)

# Package requirement
find_package(Boost 1.40 REQUIRED COMPONENTS program_options)

# Include src files to compile
file(GLOB_RECURSE srcs
        "${PROJECT_SOURCE_DIR}/*.hh"
        "${PROJECT_SOURCE_DIR}/*.cc"
        )

# Compile sources
add_executable(AnalyticalAstra ${srcs})

# Link libraries
target_link_libraries(AnalyticalAstra LINK_PUBLIC AstraSim)
target_link_libraries(AnalyticalAstra LINK_PRIVATE Boost::program_options)

# Resulting binary location settings
set_target_properties(AnalyticalAstra
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "bin/"
        LIBRARY_OUTPUT_DIRECTORY "lib/"
        ARCHIVE_OUTPUT_DIRECTORY "lib/"
        )
