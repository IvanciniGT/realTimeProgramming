
#include <stdbool.h>
#include <stdio.h>
#include "main_system_sm.h"

MainSystemState get_next_state(MainSystemState currentState, MainSystemStateTransition event) {
    switch (currentState) {
        case TURNED_OFF:
            switch (event) {
                case SYSTEM_TURN_ON: return TURNED_OFF;
                case FAILURE_DETECTED: return FAILURE;
                case SHUTDOWN: return SHUTTING_DOWN;
                default: break;
            }
            break;
        case TURNED_ON:
            switch (event) {
                case FAILURE_DETECTED: return FAILURE;
                case SHUTDOWN: return SHUTTING_DOWN;
                default: break;
            }
            break;
        case FAILURE:
            switch (event) {
                case FAILURE_SOLVED: return TURNED_OFF;
                case SHUTDOWN: return SHUTTING_DOWN;
                default: break;
            }
            break;
        case SHUTTING_DOWN:
            break;
    }

    return -1;
}


bool can_transition_be_executed(MainSystemState current_state, MainSystemStateTransition transition) {
    // make use of transition_to
    return get_next_state(current_state, transition) != -1;
}

// Right here the state machine is defined
int transition_to(MainSystemState *current_state, MainSystemStateTransition transition) {
    if (!can_transition_be_executed(*current_state, transition)) {
        printf("Invalid transition. State remains unchanged.\n");
        return 1;  // Early return on invalid transition
    }
    *current_state = get_next_state(*current_state, transition);
    return 0;
}