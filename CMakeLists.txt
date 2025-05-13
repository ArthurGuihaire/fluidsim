cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----- directories ----------------------------------------------------------
set(SOURCE_DIR  "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# ----- sources --------------------------------------------------------------
add_executable(MyApp
    ${SOURCE_DIR}/application.cpp
    ${SOURCE_DIR}/glad.c
)

# ----- headers --------------------------------------------------------------
target_include_directories(MyApp PRIVATE
    ${INCLUDE_DIR}        # glad.h  + KHR/…
)

# ----- GLAD needs OpenGL ----------------------------------------------------
find_package(OpenGL REQUIRED)
target_link_libraries(MyApp PRIVATE OpenGL::GL)

# ----- GLFW -----------------------------------------------------------------
# This finds /usr/lib/cmake/glfw3/glfw3Config.cmake (Arch, Ubuntu, etc.)
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(MyApp PRIVATE glfw)

# ---------------------------------------------------------------------------
