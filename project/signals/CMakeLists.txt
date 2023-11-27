cmake_minimum_required(VERSION 3.26)
project(signals C)

set(CMAKE_C_STANDARD 11)

add_executable(signals sender.c)
#add_executable(signals forking.c)

project(signals2 C)
add_executable(signals2 receiver.c)
