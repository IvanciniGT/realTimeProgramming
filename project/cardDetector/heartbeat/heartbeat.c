
#include <sys/signal.h>
#include <signal.h>
#include <pthread.h>

#include "heartbeat.h"
#include "../card_detector/card_detector.h"

int const HEARTBEAT_SIGNAL = SIGUSR1;// SIGRTMAX;

pid_t parent_process_to_send_heartbeat;
pthread_t heartbeat_thread;

void *heartbeat (void *arg) {
    // Let's sleep another 100ms before sending the first signal using nanosleep
    struct timespec const HEARTBEAT_FREQUENCY = {0, 100000000}; // 100ms
    while (current_state != SHUTDOWN) {
        // Send the signal to the parent process
        kill(parent_process_to_send_heartbeat, HEARTBEAT_SIGNAL);
        // Wait 100ms
        nanosleep(&HEARTBEAT_FREQUENCY, NULL);
    }
    return NULL;
}

// configure thread to run the heartbeat... and return the thread
void configure_heartbeat_thread(pid_t parentProcess) {
    parent_process_to_send_heartbeat = parentProcess;

    // Initialize thread attributes
    pthread_attr_t attr;

    struct sched_param param;
    pthread_attr_init(&attr);
    // Set thread scheduling policy to SCHED_FIFO (First In, First Out)
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    // Set thread priority to 50 (you can adjust this as needed)
    param.sched_priority = 50;
    pthread_attr_setschedparam(&attr, &param);
    // Create the thread

    pthread_create(&heartbeat_thread, &attr, heartbeat, NULL);
    // Destroy the thread attributes
    pthread_attr_destroy(&attr);
}

void join_heartbeat_thread() {
    pthread_join(heartbeat_thread, NULL);
}
