cmake_minimum_required(VERSION 3.14)
project(03)
set(binary src/main.cpp src/Shader.cpp src/stb_image.cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(03 ${binary} src/glad.c)

add_compile_options(-std=c++17)

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(03 glfw)

include_directories(/usr/local/include/GLFW ./include)