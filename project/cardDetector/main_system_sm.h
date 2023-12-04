#ifndef CARD_DETECTION_SM_H
#define CARD_DETECTION_SM_H

#include <stdbool.h>

// Enumerations for states and transitions
typedef enum {
    TURNED_OFF,
    TURNED_ON,
    FAILURE,
    SHUTTING_DOWN
} MainSystemState;

typedef enum {
    SYSTEM_TURN_ON,
    SYSTEM_TURN_OFF,
    SHUTDOWN,
    FAILURE_DETECTED,
    FAILURE_SOLVED
} MainSystemStateTransition;

// Function prototypes
bool can_transition_be_executed(MainSystemState current_state, MainSystemStateTransition transition) ;
int transition_to(MainSystemState *current_state, MainSystemStateTransition transition) ;

#endif // CARD_DETECTION_SM_H