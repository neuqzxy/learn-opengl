cmake_minimum_required(VERSION 3.14)
project(06)

set(binary src/Shader.cpp src/stb_image.cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(jmfs ${binary} src/jmfs.cpp src/glad.c)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(jmfs glfw)
include_directories(./include)
