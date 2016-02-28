# In The Name Of God
# ========================================
# [] File Name : CMakeLists.txt
#
# [] Creation Date : 28-02-2016
#
# [] Created By : Parham Alvani (parham.alvani@gmail.com)
# =======================================
add_executable(LED.out main.c
	util.h
	user.h user.c)

include_directories("${LED_SOURCE_DIR}/libs/src")
target_link_libraries(LED.out kaac crypto)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g -Wall")
