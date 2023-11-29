#include <stdio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <libc.h>
#include <sys/semaphore.h>

#define SHARED_MEMORY_NAME "/card_id"
#define SEMAPHORE_NAME "/card_id_semaphore"
#define CARD_ID_SIZE 16

struct SharedData {
    u_int8_t card_id[CARD_ID_SIZE];
    int flag;
    pthread_mutex_t mutex;
};

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


void *listening_for_events_thread(void *arg){
    // We need to listen for events (new CardId detected)
    // We need to send a signal to the vital sign monitor thread
    // We need to send the CardId to the vital sign monitor thread
    while(1){ // If The status is not TURN_OFF
        // We need to listen for events (new CardId detected)
        // We need to send a signal to the vital sign monitor thread
        // We need to send the CardId to the vital sign monitor thread
        // Make sure that only we can access the shared memory to store the cardId
        sem_wait(semaphore);
        pthread_mutex_lock(&shared_data->mutex); // Let's get a lock on the mutex
        // We want to store the cardId in the shared memory
        u_int8_t cardId[CARD_ID_SIZE] ;
        memccpy(cardId, shared_data->card_id, 0, CARD_ID_SIZE);
        shared_data->flag = 0;
        pthread_mutex_unlock(&shared_data->mutex); // Let's release the lock on the mutex
        // We want to notify the card detector process that we have a new cardId in the shared memory

        // We may want to open a new thread to send the request to this central server
        // We will make sure that no other request is being sent to the central server
        // We will configure a timeout for the request to the central server

    }
}


int main() {
    init_system();
    // We will have a this main thread for the vital sign monitor
    // We want another thread for listening for events (new CardId detected)
    pthread_t events_listener_thread;

    pthread_create(&events_listener_thread, NULL, listening_for_events_thread, NULL);

    while(1){ // If The status is not SHUTDOWN
        usleep(1);
        printf("I am alive\n");
    }

}
