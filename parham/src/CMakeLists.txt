# In The Name Of God
# ========================================
# [] File Name : CMakeLists.txt
#
# [] Creation Date : 22-02-2016
#
# [] Created By : Parham Alvani (parham.alvani@gmail.com)
# =======================================
add_executable(Parham.out main.c
	util.h
	notification.h notification.c
	event.h event.c
	user.h user.c)

include_directories("${Parham_SOURCE_DIR}/libs/src")
target_link_libraries(Parham.out kaac crypto)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99 -g -Wall")
