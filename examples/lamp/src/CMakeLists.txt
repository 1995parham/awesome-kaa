add_executable(lamp.out main.c)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${lamp_SOURCE_DIR}/kaa/src")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g -Wall")
