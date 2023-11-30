cmake_minimum_required(VERSION 3.26)
project(Sender C)

set(CMAKE_C_STANDARD 11)

add_executable(Sender sender.c)

project(Logger C)
add_executable(Logger logger.c)
