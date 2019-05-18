cmake_minimum_required(VERSION 3.14)
project(OMPChat C)

set(CMAKE_C_STANDARD 99)

find_package(Threads REQUIRED)

add_executable(OMPChat src/main.c src/client.c src/client.h src/server.c src/server.h src/common.c src/common.h)

TARGET_LINK_LIBRARIES(OMPChat Threads::Threads)