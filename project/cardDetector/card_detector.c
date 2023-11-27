#include <stdio.h>
#include "card_detector_sm.h"
#include "card_detector.h"

CardDetectionState current_state = OFF;

int change_state_when_possible(CardDetectionStateTransition transition) {
    int turn_on_result = 1;
    if (can_transition_be_executed(current_state, transition)) {
        turn_on_result = transition_to(&current_state, transition);
    }
    return turn_on_result;
}

// turn on the system
int turn_on() {
    return change_state_when_possible(SYSTEM_TURN_ON);
}

int turn_off() {
    return change_state_when_possible(SYSTEM_TURN_OFF);
}

int positive_detection() {
    return change_state_when_possible(POSITIVE_DETECTION);
}

int negative_detection() {
    return change_state_when_possible(NEGATIVE_DETECTION);
}

int failure_detection() {
    return change_state_when_possible(FAILURE_DETECTION);
}

int failure_solved() {
    return change_state_when_possible(FAILURE_SOLVED);
}
