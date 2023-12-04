//
// Created by Ivan Osuna Ayuste on 27/11/23.
//

#ifndef CARDDETECTOR_CARD_DETECTOR_H
#define CARDDETECTOR_CARD_DETECTOR_H

#include "card_detector_sm.h"
#include "../card_validator_shared_memory/card_validator_shared_memory.h"

extern MainSystemState current_state;
int init_system();
int turn_on();
int turn_off();
int positive_detection(const u_int8_t carId[CARD_ID_SIZE]);
int negative_detection();
int failure();
int failure_solved();
int shutdown_system();
int final_system_cleanup();
#endif //CARDDETECTOR_CARD_DETECTOR_H
