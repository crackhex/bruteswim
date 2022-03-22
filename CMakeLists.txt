cmake_minimum_required(VERSION 3.21)
project(bruteswim C)

set(CMAKE_C_STANDARD 23)

add_executable(bruteswim main.c maths.c maths.h types.h trig_table.c mario.c mario.h surface.c surface.h swimming.c swimming.h)
target_link_libraries(bruteswim m)