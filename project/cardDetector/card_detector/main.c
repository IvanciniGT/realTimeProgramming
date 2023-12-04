#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "card_detector.h"
#include "../basic_control_signals/basic_control_signals.h"
#include "../heartbeat/heartbeat.h"

struct timespec const VITALITY_FREQUENCY = {0, 100000000}; // 100ms

int main(int argc, char *argv[]) {
    // We need to know the parent process id
    if(argc != 2) {
        printf("Usage: %s <parent_process_id>\n", argv[0]);
        return 1;
    }
    long inputPid = strtol(argv[1], NULL, 10);
    pid_t parentProcessId = (pid_t) inputPid;
    printf("Initializing card detector with parent process id: %d\n", parentProcessId);
    init_system();
    // The parent process is gonna be able to communicate with this process through signals
    configure_signal_handlers();
    printf("System initialized\n");

    // Init the heartbeat thread
    configure_heartbeat_thread(parentProcessId);

    // Are we going to send any information (to communicate) to the parent process?
    while(SHUTTING_DOWN != current_state) {
        nanosleep(&VITALITY_FREQUENCY, NULL);
    }

    // Join the heartbeat thread
    join_heartbeat_thread();
    final_system_cleanup();
}