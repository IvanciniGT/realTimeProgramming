
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "main_system_sm.h"
#include "heartbeat_signals_handlers.h"
#include "subsystems.h"

MainSystemState current_state = TURNED_OFF;
struct timespec const HEARTBEAT_CHECKS_INTERVAL = {0, 200000000}; // 100ms

int retries = 3;

int change_state_when_possible(MainSystemStateTransition transition) {
    int turn_on_result = 1;
    if (can_transition_be_executed(current_state, transition)) {
        turn_on_result = transition_to(&current_state, transition);
    }
    return turn_on_result;
}


int init_system() {
    if(start_subsystems()>0) {
        perror("Error starting the subsystems");
        return change_state_when_possible(FAILURE_DETECTED);
    }
    return 0;
}
int turn_on() {
    int done = change_state_when_possible(SYSTEM_TURN_ON);
    if(done == 0) {
        // Should send signals to subsystems to turn on
        turn_on_subsystems();
    }
    return done;
}

int turn_off() {
    int done = change_state_when_possible(SYSTEM_TURN_OFF);
    if(done == 0) {
        // Should send signals to subsystems to turn off
        turn_off_subsystems();
    }
    return done;
}

int failure_solved() {
    int done = change_state_when_possible(FAILURE_SOLVED);
    if(done == 0) {
        turn_on_subsystems();
    }
    return done;
}

int main() {

    // We want to configure some listeners for some signals (Heartbeat)
    configure_heartbeat_handlers();
    // Case we do not receive a heartbeat in a certain amount of time we will mark the system as failure
    // We will wait a bit more to receive a heartbeat... case we don't we will restart these subprocesses
    // We will wait a little bit to se it we receive a heartbeat... case we don't we will retry that work once and again

    // Open a couple of suprocesses:
    // 1 for the card validator
    // 1 for the card detector

    init_system();
    if(current_state == FAILURE) {
        perror("Error starting the subsystems");
        return 1;
    }
    if(turn_on()>0) {
        perror("Error starting the subsystems");
        return 1;
    }
    while(current_state!= SHUTDOWN) {
        // Here is when we want to start listening for heartbeats

        if (current_state == TURNED_ON){
                if(are_all_subprocesses_alive()>0) {
                    change_state_when_possible(FAILURE_DETECTED);
                }

        }
        if (current_state == FAILURE){
            if(are_all_subprocesses_alive()>0) {
                change_state_when_possible(FAILURE_DETECTED);
                if(retries>0) {
                    retries--;
                } else {
                    restart_subsystems();
                    usleep(1000000); // To make sure that the subprocesses are ready
                    retries = 3;
                }
            }else{
                failure_solved();
                retries = 3;
            }
        }
        nanosleep(&HEARTBEAT_CHECKS_INTERVAL, NULL);
    }

    // We will turn on them

}

