add_executable(demo.out main.c)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${license-plate_SOURCE_DIR}/kaa/src")
target_link_libraries(license-plate.out kaac crypto ${CMAKE_THREAD_LIBS_INIT})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g -Wall")
