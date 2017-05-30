cmake_minimum_required(VERSION 2.8.12)
project(kaa-application C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g -Wall -Wextra")

add_subdirectory(kaa)

add_executable(kaa-app main.c)

target_link_libraries(kaa-app kaac)

