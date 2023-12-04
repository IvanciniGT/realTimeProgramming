//
// Created by Ivan Osuna Ayuste on 27/11/23.
//

#ifndef CARDVALIDATOR_CARD_VALIDATOR_H
#define CARDVALIDATOR_CARD_VALIDATOR_H

#include <semaphore.h>
#include "card_validator_sm.h"
#include "../card_validator_shared_memory/card_validator_shared_memory.h"


extern CardDetectionState current_state;
int init_system();
int turn_on();
int turn_off();
int card_detected(u_int8_t cardId[CARD_ID_SIZE]);
int failure();
int failure_solved();
int shutdown_system();
int final_system_cleanup();
#endif //CARDVALIDATOR_CARD_VALIDATOR_H
