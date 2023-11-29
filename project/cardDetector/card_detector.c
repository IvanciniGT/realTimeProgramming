#include <stdio.h>
#include <pthread.h>
#include "card_detector_sm.h"
#include "card_detector.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <libc.h>
#include <sys/semaphore.h>

#define SHARED_MEMORY_NAME "/card_id"
#define SEMAPHORE_NAME "/card_id_semaphore"

struct SharedData {
    u_int8_t card_id[CARD_ID_SIZE];
    int flag;
    pthread_mutex_t mutex;
};

CardDetectionState current_state = OFF;
int shared_memory_fd;
struct SharedData *shared_data;
sem_t *semaphore;

int init_system() {
    // Open or create the shared memory
                                // v It is just the name of the shared memory
                                                    // v Create if it does not exist and anyway open it
                                                              // v Read and write permissions
                                                                      // Specific permissions for the owner / group / others
    shared_memory_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
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
    return 0;
}

int change_state_when_possible(CardDetectionStateTransition transition) {
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

// We will receive a call to this function from the sensor driver
int positive_detection(const u_int8_t carId[CARD_ID_SIZE]){
    // Make sure that only we can access the shared memory to store the cardId
    pthread_mutex_lock(&shared_data->mutex); // Let's get a lock on the mutex
    // We want to store the cardId in the shared memory
    memccpy(shared_data->card_id, carId, 0, CARD_ID_SIZE);
    shared_data->flag = 1;
    pthread_mutex_unlock(&shared_data->mutex); // Let's release the lock on the mutex
    // We want to notify the card detector process that we have a new cardId in the shared memory
    sem_post(semaphore);

    return change_state_when_possible(POSITIVE_DETECTION);
}

int negative_detection() {
    return change_state_when_possible(NEGATIVE_DETECTION);
}

int failure_detection() {
    return change_state_when_possible(FAILURE_DETECTION);
}

int failure_solved() {
    return change_state_when_possible(FAILURE_SOLVED);
}
