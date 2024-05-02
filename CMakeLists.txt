cmake_minimum_required(VERSION 3.7.2)
project(Bhaga C)

set(CMAKE_C_STANDARD 99)

add_executable(
        Bhaga main.c
        main.h
        sub.c
        sub.h
        map.c
        map.h
        semaphore.c
        semaphore.h
        messages.h
        messages.c
        subscriptions.h
        subscriptions.c
)

target_link_libraries(Bhaga pthread)
