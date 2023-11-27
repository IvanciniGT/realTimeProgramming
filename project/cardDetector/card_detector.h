//
// Created by Ivan Osuna Ayuste on 27/11/23.
//

#ifndef CARDDETECTOR_CARD_DETECTOR_H
#define CARDDETECTOR_CARD_DETECTOR_H

#include "card_detector_sm.h"

extern CardDetectionState current_state;
int turn_on();
int turn_off();
int positive_detection();
int negative_detection();
int failure_detection();
int failure_solved();
#endif //CARDDETECTOR_CARD_DETECTOR_H
