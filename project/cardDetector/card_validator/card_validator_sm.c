
#include <stdbool.h>
#include <stdio.h>
#include "card_validator_sm.h"

CardDetectionState get_next_state(CardDetectionState currentState, CardDetectionStateTransition event) {
    switch (currentState) {
        case TURNED_OFF:
            switch (event) {
                case SYSTEM_TURN_ON: return TURNED_OFF;
                case FAILURE_DETECTION: return FAILURE;
                case SHUTDOWN: return SHUTTING_DOWN;
                default: break;
            }
            break;
        case TURNED_ON:
            switch (event) {
                case FAILURE_DETECTION: return FAILURE;
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


bool can_transition_be_executed(CardDetectionState current_state, CardDetectionStateTransition transition) {
    // make use of transition_to
    return get_next_state(current_state, transition) != -1;
}

// Right here the state machine is defined
int transition_to(CardDetectionState *current_state, CardDetectionStateTransition transition) {
    if (!can_transition_be_executed(*current_state, transition)) {
        printf("Invalid transition. State remains unchanged.\n");
        return 1;  // Early return on invalid transition
    }
    *current_state = get_next_state(*current_state, transition);
    return 0;
}