cmake_minimum_required(VERSION 3.14)

project(game)

set(sources src/glad.c src/Shader.cpp src/Texture.cpp src/Game.cpp src/stb_image.cpp src/ResourceManager.cpp src/SpriteRender.cpp src/GameObject.cpp src/GameLevel.cpp)
set(CMAKE_CXX_STANDARD 14)

add_executable(game ${sources} src/main.cpp)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(game glfw)
include_directories(./include)
