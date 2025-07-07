cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----- directories ----------------------------------------------------------
set(SOURCE_DIR  "${CMAKE_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

# ----- headers --------------------------------------------------------------
include_directories(${INCLUDE_DIR})

# ----- Dependencies ---------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# ----- Common sources -------------------------------------------------------
file(GLOB COMMON_SOURCES "${SOURCE_DIR}/common/*.cpp" "${SOURCE_DIR}/common/*.c")
message(STATUS "COMMON_SOURCES = ${COMMON_SOURCES}")

# ----- BasicShader -----------------------------------------------------------------
set(BasicShader_SOURCES
    ${SOURCE_DIR}/application.cpp
    ${COMMON_SOURCES}
)

add_executable(BasicShader ${BasicShader_SOURCES})
target_include_directories(BasicShader PRIVATE ${INCLUDE_DIR})
target_link_libraries(BasicShader PRIVATE OpenGL::GL glfw)

# ----- TextureShader -----------------------------------------------------------------
set(TextureShader_SOURCES
    ${SOURCE_DIR}/textureTest.cpp
    ${COMMON_SOURCES}
)

add_executable(TextureShader ${TextureShader_SOURCES})
target_include_directories(TextureShader PRIVATE ${INCLUDE_DIR})
target_link_libraries(TextureShader PRIVATE OpenGL::GL glfw)

# ----- Copy shaders directory -----------------------------------------------
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})
