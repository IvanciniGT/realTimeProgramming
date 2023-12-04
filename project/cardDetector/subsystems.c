
#include <signal.h>
#include "subsystems.h"
#include "basic_control_signals/basic_control_signals.h"

int start_subsystems() {
    // Here we will open the subprocesses: card validator and card detector
    // fork()
    return 0;
}

int restart_subsystems(){
    return 0;
}

void turn_on_subsystems() {
    // Should send signals to subsystems to turn on
    kill(card_validator_pid, TURN_ON_SIGNAL);
    kill(card_detector_pid, TURN_ON_SIGNAL);
}
void turn_off_subsystems() {
    // Should send signals to subsystems to turn on
    kill(card_validator_pid, TURN_OFF_SIGNAL);
    kill(card_detector_pid, TURN_OFF_SIGNAL);
}
void shutdown_subsystems() {
    // Should send signals to subsystems to turn on
    kill(card_validator_pid, SHUTDOWN_SIGNAL);
    kill(card_detector_pid, SHUTDOWN_SIGNAL);
}