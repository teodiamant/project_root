cmake_minimum_required(VERSION 3.10)
project(Utils)

# Create a library from the utils source files
add_library(utils utils.cpp)

# Include directories for the utils library
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../../includes/custom)

# Link the utils library with CGAL if necessary
target_link_libraries(utils CGAL::CGAL)
