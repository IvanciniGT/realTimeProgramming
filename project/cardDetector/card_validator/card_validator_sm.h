#ifndef CARD_DETECTION_SM_H
#define CARD_DETECTION_SM_H

#include <stdbool.h>

// Enumerations for states and transitions
typedef enum {
    TURNED_OFF,
    TURNED_ON,
    FAILURE,
    SHUTTING_DOWN
} CardDetectionState;

typedef enum {
    SYSTEM_TURN_ON,
    SYSTEM_TURN_OFF,
    SHUTDOWN,
    FAILURE_DETECTION,
    FAILURE_SOLVED
} CardDetectionStateTransition;

// Function prototypes
bool can_transition_be_executed(CardDetectionState current_state, CardDetectionStateTransition transition) ;
int transition_to(CardDetectionState *current_state, CardDetectionStateTransition transition) ;

#endif // CARD_DETECTION_SM_H