cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set source directory
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Add executable
add_executable(MyApp "${SOURCE_DIR}/application.cpp")

# Find GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include GLFW and OpenGL headers
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

# Link GLFW and OpenGL
target_link_libraries(MyApp ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY})
