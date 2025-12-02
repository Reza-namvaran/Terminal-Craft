cmake_minimum_required(VERSION 3.26.1)

project(terminal_craft)

include_directories(
    ./include
)

add_executable(${PROJECT_NAME} ./src/main.c)