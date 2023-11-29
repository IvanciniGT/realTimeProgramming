cmake_minimum_required(VERSION 3.26)
project(cardDetector C)

set(CMAKE_C_STANDARD 11)

add_executable(cardDetector main.c
        card_detector_sm.h
        card_detector_sm.c
        card_detector.h
        card_detector.c
        card_detector_signals.c
        card_detector_heartbeat.c
        card_detector_heartbeat.h
        NOT_HERE_card_validation_process.c)
