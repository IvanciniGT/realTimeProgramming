
#include <stdbool.h>
#include <stdio.h>
#include "card_detector_sm.h"

CardDetectionState get_next_state(CardDetectionState currentState, CardDetectionStateTransition event) {
    switch (currentState) {
        case OFF:
            switch (event) {
                case SYSTEM_TURN_ON: return ON_NEGATIVE;
                case FAILURE_DETECTION: return FAILURE;
                default: break;
            }
            break;
        case ON_POSITIVE:
            switch (event) {
                case NEGATIVE_DETECTION: return ON_NEGATIVE;
                case FAILURE_DETECTION: return FAILURE;
                default: break;
            }
            break;
        case ON_NEGATIVE:
            switch (event) {
                case POSITIVE_DETECTION: return ON_NEGATIVE;
                case FAILURE_DETECTION: return FAILURE;
                case SYSTEM_SHUTDOWN: return OFF;
                default: break;
            }
            break;
        case FAILURE:
            switch (event) {
                case FAILURE_SOLVED: return OFF;
                default: break;
            }
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
        return -1;  // Early return on invalid transition
    }
    switch (transition) {
        case SYSTEM_TURN_ON:
            printf("System is turned on\n");
            break;
        case POSITIVE_DETECTION:
            printf("Positive detection\n");
            break;
        case NEGATIVE_DETECTION:
            printf("Negative detection\n");
            break;
        case SYSTEM_SHUTDOWN:
            printf("System is shutting down\n");
            break;
        case FAILURE_DETECTION:
            printf("Failure detected\n");
            break;
        case FAILURE_SOLVED:
            printf("Failure solved\n");
            break;
        default:
            break;
    }
    *current_state = get_next_state(*current_state, transition);
    return 0;
}