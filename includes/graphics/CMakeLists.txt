cmake_minimum_required(VERSION 3.10)
project(Graphics)

# Create a library from the graphics source files
add_library(graphics graphics.cpp)

# Include directories for the graphics library
target_include_directories(graphics PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../../includes/custom
    ../../includes/utils
)

# Link the graphics library with CGAL
target_link_libraries(graphics CGAL::CGAL)
