cmake_minimum_required(VERSION 3.14)
project(01)

set(binary src/main.cpp src/Shader.cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(01 ${binary} src/glad.c)

add_compile_options(-std=c++17)

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(01 glfw)

include_directories(/usr/local/include/GLFW ./includes)
