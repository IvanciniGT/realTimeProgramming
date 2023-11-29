
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
                case SYSTEM_TURN_OFF: return OFF;
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
        return 1;  // Early return on invalid transition
    }
    switch (transition) {
        case SYSTEM_TURN_ON:
            // The transition will have a set of actions that need to be executed... associated to the transition
            printf("We will check the components and tell the sensor to turn on\n");
            // Driver code(message) to turn on the sensor
            break;
        case POSITIVE_DETECTION:
            //printf("Positive detection\n");
            break;
        case NEGATIVE_DETECTION:
            //printf("Negative detection\n");
            break;
        case SYSTEM_TURN_OFF:
            printf("We will tell the sensor to turn off\n");
            break;
        case FAILURE_DETECTION:
            //printf("Failure detected\n");
            // We could set a task to monitor the failure... that could be executed each certain amount of time
            // Case the monitor task detects that the failure is solved, it will call the transition FAILURE_SOLVED
            break;
        case FAILURE_SOLVED:
            //printf("Failure solved\n");
            break;
        default:
            break;
    }
    *current_state = get_next_state(*current_state, transition);
    return 0;
}