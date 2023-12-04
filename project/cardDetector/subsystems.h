//
// Created by Ivan Osuna Ayuste on 4/12/23.
//

#ifndef CARDVALIDATOR_SUBSYSTEMS_H
#define CARDVALIDATOR_SUBSYSTEMS_H

#include <sys/types.h>

extern pid_t card_validator_pid;
extern pid_t card_detector_pid;

int start_subsystems();
int restart_subsystems();

void turn_on_subsystems() ;
void turn_off_subsystems();
void shutdown_subsystems();
#endif //CARDVALIDATOR_SUBSYSTEMS_H
