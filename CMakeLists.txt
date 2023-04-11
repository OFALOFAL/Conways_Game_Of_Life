cmake_minimum_required(VERSION 3.22)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(libraries/glfw-3.3.8)

find_package(OpenGL REQUIRED)

link_libraries(stdc++fs)

add_executable(GameOfLife libraries/glad/src/glad.c libraries/stb/stb.cpp src/main.cpp)

include_directories(libraries/glad/include)
include_directories(libraries/glm/)

target_link_libraries(GameOfLife glfw)
target_link_libraries(GameOfLife OpenGL::GL)
