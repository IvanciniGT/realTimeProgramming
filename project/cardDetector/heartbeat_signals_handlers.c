
#include <stdio.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include "heartbeat/heartbeat.h"
#include "subsystems.h"

// We want to handle signals from 2 processes: card_validator and card_detector
int card_validator_heartbeat = false;
int card_detector_heartbeat = false;
pthread_mutex_t *mutex;

void initializeMutex() {
    // Initialize mutex attributes
    pthread_mutexattr_t mutex_attr;
    // if mutex is not already initialized, initialize it
    pthread_mutex_init(mutex, &mutex_attr);
}

void heartBeat_handler(int signo, siginfo_t *info, void *extra) {
    // Let's take a lock on the mutex
    pthread_mutex_lock(mutex);
    // Sender: info->si_pid
    if(info->si_pid == card_validator_pid) {
        card_validator_heartbeat = true;
    } else if(info->si_pid == card_detector_pid) {
        card_detector_heartbeat = true;
    }
    // Let's release the lock on the mutex
    pthread_mutex_unlock(mutex);
}

void configure_heartbeat_handlers() {
    initializeMutex();
    struct sigaction turn_on_action;
    turn_on_action.sa_sigaction = heartBeat_handler;
    turn_on_action.sa_flags = SA_SIGINFO;           // Use the extended signal handler... in order to get the PID of the sender
    sigaction(HEARTBEAT_SIGNAL, &turn_on_action, NULL);
}

int are_all_subprocesses_alive() {
    // Let's take a lock on the mutex
    pthread_mutex_lock(mutex);
    bool all_alive = card_validator_heartbeat && card_detector_heartbeat;
    card_validator_heartbeat = false;
    card_detector_heartbeat = false;
    // Let's release the lock on the mutex
    pthread_mutex_unlock(mutex);
    // Mutex here
    return all_alive;
}