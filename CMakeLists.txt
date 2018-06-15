#cmake_minimum_required(VERSION 3.10)
project(Wet C)

set(CMAKE_C_STANDARD 99)
add_executable(Wet main.c mtmflix.c mtm_ex3.h mtmflix.h utilities.c utilities.h user.c user.h series.h series.c tests/test_utilities.h)
target_link_libraries(Wet ${CMAKE_SOURCE_DIR}/libmtm.a ${CMAKE_SOURCE_DIR}/libmtm_ex3.a)
set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -pedantic-errors")