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

CardDetectionState current_state = TURNED_OFF;
int shared_memory_fd;
struct SharedData *shared_data;
sem_t *semaphore;

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
    // Mutex is not initialized, initialize it
    if (pthread_mutex_trylock(&shared_data->mutex) == 0) {
        initializeMutex();
        pthread_mutex_unlock(&shared_data->mutex);
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
int min(int a, int b) {
    return a < b ? a : b;
}
int shutdown_system() {
    if (!change_state_when_possible(SHUTDOWN)) return 1;
    // We may need to wait a little bit for the card detector to turn off
    // We may need to disconnect the sensor driver
    // Cleanup the shared memory
    int status = munmap(shared_data, sizeof(struct SharedData));
    status = min( status, shm_unlink(SHARED_MEMORY_NAME));
    status = min(status, sem_close(semaphore));
    status = min( status, sem_unlink(SEMAPHORE_NAME));
    exit(status);
}

// We will receive a call to this function from the sensor driver
int positive_detection(const u_int8_t cardId[CARD_ID_SIZE]){
    // Make sure that only we can access the shared memory to store the cardId
    pthread_mutex_lock(&shared_data->mutex); // Let's get a lock on the mutex
    // We want to store the cardId in the shared memory
    memccpy(shared_data->card_id, cardId, 0, CARD_ID_SIZE);
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
