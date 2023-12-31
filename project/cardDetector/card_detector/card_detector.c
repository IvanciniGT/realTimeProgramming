#include <stdio.h>
#include <pthread.h>
#include "card_detector_sm.h"
#include "card_detector.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define SHARED_MEMORY_NAME "/card_id"
#define SEMAPHORE_NAME "/card_id_semaphore"

MainSystemState current_state = TURNED_OFF;

void initializeMutex() {
    // Initialize mutex attributes
    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr); // Default attributes
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED); // Shared between processes
    // if mutex is not already initialized, initialize it
    pthread_mutex_init(&shared_data->mutex, &mutex_attr);
}

int init_system() {
    // Open or create the shared memory
                                // v It is just the name of the shared memory
                                                    // v Create if it does not exist and anyway open it
                                                              // v Read and write permissions
                                                                      // Specific permissions for the owner / group / others
    // Let's open the shared memory if it exists, otherwise wait until it is created
    // Let's look for a ENOENT error (No such file or directory) and in that case wait 50 ms and retry a maximum of 10 times
    int retry = 0;
    while ((shared_memory_fd = shm_open(SHARED_MEMORY_NAME, O_RDWR, S_IRUSR | S_IWUSR)) == -1 && errno == ENOENT && retry < 10) {
        usleep(500000);
        retry++;
    }
    if (shared_memory_fd == -1) {
        perror("Error opening the shared memory");
        return 1;
    }
    if (shared_memory_fd == -1) {
        perror("Error opening the shared memory");
        return 1;
    }
    // Set the size of the shared memory
    if (ftruncate(shared_memory_fd, sizeof(struct SharedData)) == -1) {
        perror("Error setting the size of the shared memory");
        return 1;
    }
    // Map the shared memory in the address space of the process
    shared_data = mmap(NULL, sizeof(struct SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("Error mapping the shared memory in the address space of the process");
        return 1;
    }
    // Create the semaphore (if it does not exist) and open it
    semaphore = sem_open(SEMAPHORE_NAME, O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(semaphore == SEM_FAILED) {
        perror("Error creating the semaphore");
        return 1;
    }
    // Mutex is not initialized, initialize it
    if (pthread_mutex_trylock(&shared_data->mutex) == 0) {
        initializeMutex();
        pthread_mutex_unlock(&shared_data->mutex);
    }
    return 0;
}

int change_state_when_possible(MainSystemStateTransition transition) {
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
int shutdown_system() {
    return change_state_when_possible(SHUTDOWN);
}

// We will receive a call to this function from the sensor driver
int positive_detection(const u_int8_t cardId[CARD_ID_SIZE]){
    if(current_state != TURNED_ON) {
        return 1;
    }
    // Make sure that only we can access the shared memory to store the cardId
    pthread_mutex_lock(&shared_data->mutex); // Let's get a lock on the mutex
    // We want to store the cardId in the shared memory
    memccpy(shared_data->card_id, cardId, 0, CARD_ID_SIZE);
    shared_data->flag = 1;
    pthread_mutex_unlock(&shared_data->mutex); // Let's release the lock on the mutex
    // We want to notify the card detector process that we have a new cardId in the shared memory
    sem_post(semaphore);
    return 0;
}

int negative_detection() {
    return current_state != TURNED_ON;
}

int failure() {
    return change_state_when_possible(FAILURE_DETECTED);
}

int failure_solved() {
    return change_state_when_possible(FAILURE_SOLVED);
}

int min(int a, int b) {
    return a < b ? a : b;
}
int final_system_cleanup(){
    // We may need to wait a little bit for the card detector to turn off
    // We may need to disconnect the sensor driver
    // Cleanup the shared memory
    int status = munmap(shared_data, sizeof(struct SharedData));
    status = min( status, shm_unlink(SHARED_MEMORY_NAME));
    status = min(status, sem_close(semaphore));
    status = min( status, sem_unlink(SEMAPHORE_NAME));
    exit(status);
}
