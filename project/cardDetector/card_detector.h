//
// Created by Ivan Osuna Ayuste on 27/11/23.
//

#ifndef CARDDETECTOR_CARD_DETECTOR_H
#define CARDDETECTOR_CARD_DETECTOR_H

#include "card_detector_sm.h"

#define CARD_ID_SIZE 16

extern CardDetectionState current_state;
int init_system();
int turn_on();
int turn_off();
int positive_detection(const u_int8_t carId[CARD_ID_SIZE]);
int negative_detection();
int failure_detection();
int failure_solved();
#endif //CARDDETECTOR_CARD_DETECTOR_H
