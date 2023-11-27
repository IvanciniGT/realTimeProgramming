#include <stdio.h>
#include "card_detector_sm.h"
#include "card_detector.h"

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
    if (can_transition_be_executed(current_state, transition)) {
        // If the transition is valid, we will execute the transition
        printf("Card Detector is turning on\n");
        int turn_on_result = transition_to(&current_state, transition);
        if (turn_on_result == -1) {
            printf("Error turning on the Card Detector\n");
            return -1;
        } else {
            printf("Card Detector is on\n");
            return 0;
        }
    }
    return 0;
}
