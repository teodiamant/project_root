cmake_minimum_required(VERSION 3.1...3.23)

project(project_root)

# CGAL and its components
find_package(CGAL QUIET COMPONENTS Qt5)

if (NOT CGAL_FOUND)
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()
endif()

# Boost and its components
find_package(Boost REQUIRED)

if (NOT Boost_FOUND)
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()
endif()

# Include directories
include_directories(${Boost_INCLUDE_DIRS} ${CGAL_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(includes/utils)
add_subdirectory(includes/graphics)

# Create the executable target
add_executable(project_root ex.cpp)

# Include directories for custom and other headers
target_include_directories(project_root PRIVATE
  includes/custom
  includes/utils
  includes/graphics
)

# Link to the utils and graphics libraries and the external libraries
target_link_libraries(project_root utils graphics CGAL::CGAL ${Boost_LIBRARIES})

# Link Qt5 libraries only if CGAL_Qt5 is found
if (CGAL_Qt5_FOUND)
  add_definitions(-DCGAL_USE_BASIC_VIEWER)
  target_link_libraries(project_root CGAL::CGAL_Qt5)
endif()
