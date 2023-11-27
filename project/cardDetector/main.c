#include <stdio.h>
#include <time.h>
#include "card_detector.h"
#include "card_detector_signals.h"
#include "card_detector_heartbeat.h"

struct timespec const VITALITY_FREQUENCY = {0, 100000000}; // 100ms
int main() {
    pid_t parentProcessId ; // We will read this information from the command line arguments
    printf("Card Detector Subsystem\n");
    // The parent process is gonna be able to communicate with this process through signals
    configure_signal_handlers();
    // Are we going to send any information (to communicate) to the parent process?
    while(true) {
        // Heartbeat: Hey... I'm alive... and I'm doing my job
        // Imagine this process deliver this signal ^^^5 times and the OS has not allowed delivery any of them to the parent process
        // Do we want to deliver the signal 5 times? Or we just want to deliver the latest one? Just the lastONE: RT signal is gonna work perfectly here
        heartbeat( parentProcessId );
        nanosleep(&VITALITY_FREQUENCY, NULL);
    }
}


// MASTER PROCESS: WHOLE SYSTEM
// That master process is going to spawn (launch) all the other processes... like this one.
// But in case a subprocess dies unexpectedly... the master process needs to know that...
// It may need to mark the whole system as FAILURE
// And it may try to fix the issue... by restarting the subprocess