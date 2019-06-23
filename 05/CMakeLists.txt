cmake_minimum_required(VERSION 3.14)
project(05)
set(binary  src/Shader.cpp src/stb_image.cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(05 ${binary} src/main.cpp src/glad.c)
add_executable(05_01 ${binary} src/main_01.cpp src/glad.c)
add_executable(hj ${binary} src/HJ_light.cpp src/glad.c)
add_executable(mfs ${binary} src/mfs_light.cpp src/glad.c)
add_executable(jmfs ${binary} src/jmfs_light.cpp src/glad.c)

add_compile_options(-std=c++17)

find_package(glfw3 3.3 REQUIRED)

target_link_libraries(05 glfw)
target_link_libraries(05_01 glfw)
target_link_libraries(hj glfw)
target_link_libraries(mfs glfw)
target_link_libraries(jmfs glfw)

include_directories(./include)