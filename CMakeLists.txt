cmake_minimum_required(VERSION 3.26)
project(Bhaga C)

set(CMAKE_C_STANDARD 99)

add_executable(Bhaga main.c
        main.h
        keyValStore.c
        keyValStore.h
        sub.c
        sub.h)
