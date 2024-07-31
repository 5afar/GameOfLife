cmake_minimum_required(VERSION 3.28)
project(GameOfLive)
set(CMAKE_CXX_STANDARD 11)

set(SRC ./src/main.cpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(GameOfLive ${SRC})


target_link_libraries(GameOfLive ${SDL2_LIBRARIES})

