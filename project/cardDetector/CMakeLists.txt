cmake_minimum_required(VERSION 3.26)
set(CMAKE_C_STANDARD 11)

project(cardDetector C)
add_executable(cardDetector
        card_detector/main.c
        card_detector/card_detector_sm.h
        card_detector/card_detector_sm.c
        card_detector/card_detector.h
        card_detector/card_detector.c
        basic_control_signals/basic_control_signals.h
        basic_control_signals/basic_control_signals.c
        heartbeat/heartbeat.c
        heartbeat/heartbeat.h
        card_validator_shared_memory/card_validator_shared_memory.h
        card_validator_shared_memory/card_validator_shared_memory.c
)

project(cardValidator C)
add_executable(cardValidator
        card_validator/main.c
        card_validator/card_validator.h
        card_validator/card_validator.c
        card_validator/card_validator_sm.h
        card_validator/card_validator_sm.c
        basic_control_signals/basic_control_signals.h
        basic_control_signals/basic_control_signals.c
        heartbeat/heartbeat.c
        heartbeat/heartbeat.h
        card_validator/card_detected.c
        card_validator/card_detected.h
        card_validator/card_validation_thread_pool.c
        card_validator/card_validation_thread_pool.h
        card_validator_shared_memory/card_validator_shared_memory.c
        card_validator_shared_memory/card_validator_shared_memory.c
)