cmake_minimum_required(VERSION 2.8.8)
project(demo)
enable_language(C)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g -Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(kaa)
add_subdirectory(src)
