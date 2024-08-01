cmake_minimum_required(VERSION 3.28)
project(GameOfLife)
set(CMAKE_CXX_STANDARD 11)

set(SRC ./src/main.cpp
        ./src/game.cpp
)
include_directories(./include)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(GameOfLife ${SRC})


target_link_libraries(GameOfLife ${SDL2_LIBRARIES})

