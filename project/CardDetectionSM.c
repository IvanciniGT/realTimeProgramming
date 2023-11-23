

/* Subsystem. Card detection process

```mermaid
OFF: Off
ON_POSITIVE: On, detection (ID)
ON_NEGATIVE: On, no detection
FAILURE: Failure

OFF --> ON_NEGATIVE: System is turn on
ON_POSITIVE --> ON_NEGATIVE: Negative detection
ON_NEGATIVE --> ON_POSITIVE: Positive detection
ON_POSITIVE --> FAILURE: Failure
ON_NEGATIVE --> FAILURE: Failure
OFF --> FAILURE: Failure
ON_NEGATIVE --> OFF: Turn off
*/

#include <stdbool.h>
#include <stdio.h>
#include <time.h>


typedef enum {
    OFF,
    ON_POSITIVE,
    ON_NEGATIVE,
    FAILURE
} CardDetectionState;

typedef enum {
    SYSTEM_TURN_ON,
    POSITIVE_DETECTION,
    NEGATIVE_DETECTION,
    SYSTEM_SHUTDOWN,
    FAILURE_DETECTION,
    FAILURE_SOLVED
} CardDetectionStateTransition;

// Right here the valid transitions are defined
bool validate_transition(CardDetectionState current_state, CardDetectionStateTransition transition) {
    switch (current_state) {
        case OFF:
            return transition == SYSTEM_TURN_ON || transition == FAILURE_DETECTION;
        case ON_POSITIVE:
            return transition == NEGATIVE_DETECTION || transition == FAILURE_DETECTION;
        case ON_NEGATIVE:
            return transition == POSITIVE_DETECTION || transition == FAILURE_DETECTION || transition == SYSTEM_SHUTDOWN;
        case FAILURE:
            return transition == FAILURE_SOLVED;
        default:
            return false;
    }
}



// Right here the state machine is defined
CardDetectionState transition_to(CardDetectionState current_state, CardDetectionStateTransition transition) {
    if (!validate_transition(current_state, transition)) {
        return FAILURE;
        // TODO: Handle error... log the error, etc.
        //return current_state;
    }

    switch (current_state) {
        case OFF:
            if (transition == SYSTEM_TURN_ON) {
                return ON_NEGATIVE;
            } else if (transition == FAILURE_DETECTION) {
                return FAILURE;
            }
        case ON_POSITIVE:
            if (transition == NEGATIVE_DETECTION) {
                return ON_NEGATIVE;
            } else if (transition == FAILURE_DETECTION) {
                return FAILURE;
            }
        case ON_NEGATIVE:
            if (transition == POSITIVE_DETECTION) {
                return ON_POSITIVE;
            } else if (transition == FAILURE_DETECTION) {
                return FAILURE;
            } else if (transition == SYSTEM_SHUTDOWN) {
                return OFF;
            }
        case FAILURE:
            if (transition == FAILURE_SOLVED) {
                return OFF;
            }
        default:
            return FAILURE;
    }
}

// Here we will define the functions that will trigger the transitions...
// and the actions that should be executed along with the transition
// Those are the signals that we will receive from the other subsystems
// and the internal events that we will generate

// I need a variable to store the current state of this system
CardDetectionState current_state = OFF;

// turn on the system
int turn_on() { // When this function is going to be executed? We will be listening to a signal from the other system
    // Should we turn on the system? We will check the current state... and if that transition is valid
    CardDetectionStateTransition transition = SYSTEM_TURN_ON;

    // Is protecting myself from external bugs
    // If the one who is emiting the signal is working properly, this method ios always going to return true
    // The thing is that we don't know if the other system is working properly- I'm doing my thing.
    // And I have to make sure that nobody is going to mess with me
    // TODO: Think about how many thread are gonna be able to execute this code at the same time.
    //       That could lead to a problem
    if (validate_transition(current_state, transition)) {
    // if( current_state == OFF ) {    
        // If the transition is valid, we will execute the transition
        current_state = transition_to(current_state, transition);
        // And we will execute the actions that should be executed along with the transition
        printf("System is turned on\n");
        CardDetectionState next_state = transition_to(current_state, transition);
    }
}
