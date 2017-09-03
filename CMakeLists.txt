project(librnd)
cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 99)
include_directories(.)


add_library(rnd
        rnd.c
        rnd.h
        tmp/config.h
        )


add_executable(rnd_tests
        test/rnd_test.c
        )
target_link_libraries(rnd_tests rnd)
