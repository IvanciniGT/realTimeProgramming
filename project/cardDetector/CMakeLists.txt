cmake_minimum_required(VERSION 3.26)
project(cardDetector C)

set(CMAKE_C_STANDARD 11)

add_executable(cardDetector main.c
        card_detector_sm.h
        card_detector_sm.c
        card_detector.h
        card_detector.c)
