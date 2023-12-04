#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "card_validator.h"

#define THREAD_POOL_SIZE 10
#define QUEUE_POOL_SIZE 50

// Structure to represent a task
typedef struct {
    void (*task_function)(void*); // Function pointer for the task
    u_int8_t cardId[CARD_ID_SIZE]; // Argument for the task function
} Task;

// Structure to represent the thread pool
typedef struct {
    pthread_t threads[THREAD_POOL_SIZE];
    volatile Task task_queue[QUEUE_POOL_SIZE];        // Circular queue. We will have 20 slots for tasks
                                                // Once all slots are filled, we will start overwriting tasks
                                                // Hopefully, the thread pool will be able to process tasks faster
                                                // Case it doesn't, we will have to increase the size of the queue (if we have enough memory)
                                                // Or we will lose old tasks
    pthread_mutex_t queue_mutex;                // Mutex to protect the queue
    pthread_cond_t queue_condition;             // Condition variable to signal the availability of a new task
    volatile int head;                                   // Index of the first task that needs to be executed
    volatile int tail;                                   // Index of the next available slot in the queue
    volatile int count;                                  // Is how many tasks needs to be executed in the queue
    volatile int shutdown;
} ThreadPool;

// Global ThreadPool variable
// Volatile instructs the compiler to not optimize the variable (caching the variable for the current thread). 
// As this variable is shared between threads, and we don't want that.
// That could lead to unexpected behaviors 
ThreadPool pool;

// Function to add a task to the thread pool
void* validation_task(void* arg) {
    while (current_state != SHUTDOWN) {
        pthread_mutex_lock(&pool.queue_mutex);

        // Wait for a task if the queue is empty and the pool is not shutdown
        while (pool.count == 0 && !pool.shutdown && current_state != SHUTDOWN) {
            pthread_cond_wait(&pool.queue_condition, &pool.queue_mutex);
        }

        // If the pool is shutdown and the queue is empty, exit the thread
        if (pool.shutdown || current_state == SHUTDOWN) {
            pthread_mutex_unlock(&pool.queue_mutex);
            pthread_exit(NULL);
        }

        // Retrieve and execute the task
        Task task = pool.task_queue[pool.head];
        pool.head = (pool.head + 1) % QUEUE_POOL_SIZE; // Here we implement the circular queue
        pool.count--;

        pthread_mutex_unlock(&pool.queue_mutex);

        // Cast the argument to the expected type and call the task function
        task.task_function((void*)task.cardId);
    }
    return NULL;
}


// Function to initialize the thread pool
void configure_validation_thread_pool() {
    pthread_mutex_init(&pool.queue_mutex, NULL);
    pthread_cond_init(&pool.queue_condition, NULL);
    pool.head = 0;
    pool.tail = 0;
    pool.count = 0;
    pool.shutdown = 0;

    for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
        pthread_create(&pool.threads[i], NULL, validation_task, NULL);
    } // In this case, I did not assign a priority to the threads... We should do that....
    // Actually which priority should we assign to the threads?
    // Right now we have several priorities already:
    // Heartbeat thread: 50
    // Card detection thread: 40
    // Would it be ok to assign here a priority of 30? Could this lead us to a priority inversion problem?
    //  We are locking the mutex in the card detection thread, also in this card validation threads
    //          (those are the ones who will contact a backend service to validate the card)
    // Both they do a lock on the mutex, so they will not be able to run at the same time
    // But the card detection thread has a higher priority than the card validation thread
    // If a card validation thread is running, and the card detection thread wakes up,
    // it will not be able to run until the card validation thread finishes... but it will have higher priority
    // So the card validation thread will be able to run, and it will not be able to unlock the mutex
}



// Function to shutdown the thread pool
void clean_up_validation_thread_pool() {
    pthread_mutex_lock(&pool.queue_mutex);
    pool.shutdown = 1;
    pthread_mutex_unlock(&pool.queue_mutex);

    // Signal all waiting threads to wake up and exit
    pthread_cond_broadcast(&pool.queue_condition);

    // Wait for all threads to finish
    for (int i = 0; i < THREAD_POOL_SIZE; ++i) {
        pthread_join(pool.threads[i], NULL);
    }

    // Clean up resources
    pthread_mutex_destroy(&pool.queue_mutex);
    pthread_cond_destroy(&pool.queue_condition);
}

// Example task function
void validate_card(u_int8_t cardId[CARD_ID_SIZE]) {
    printf("Task executed with cardId: ");
    for (int i = 0; i < CARD_ID_SIZE; ++i) {
        printf("%02X ", cardId[i]);
    }
    printf("\n");
    // We would contact a central server
    // And we would fire a signal:
    // - Card validated signal
    // - Card not validated signal
}

void validate_card_wrapper(void* arg) {
    u_int8_t* cardId = (u_int8_t*)arg;
    validate_card(cardId);
}

// Function to submit a task to the thread pool
void new_card_detected_submit(void (*task_function)(void*), u_int8_t cardId[CARD_ID_SIZE]) {
    pthread_mutex_lock(&pool.queue_mutex);

    // Check if the pool is not shutdown
    if (pool.shutdown) {
        pthread_mutex_unlock(&pool.queue_mutex);
        return;
    }

    // Add the task to the queue
    pool.task_queue[pool.tail].task_function = task_function;
    memcpy(pool.task_queue[pool.tail].cardId, cardId, CARD_ID_SIZE);
    pool.tail = (pool.tail + 1) % QUEUE_POOL_SIZE; // This is the circular queue implementation
    pool.count++;

    // Signal a waiting thread that a new task is available
    pthread_cond_signal(&pool.queue_condition);

    pthread_mutex_unlock(&pool.queue_mutex);
}
void new_card_detected(u_int8_t cardId[CARD_ID_SIZE]) {
    new_card_detected_submit(validate_card_wrapper, cardId);
}



int main_test() {
    // Initialize the thread pool
    configure_validation_thread_pool();

    // Example: Submitting tasks to the thread pool
    for (int i = 0; i < 20; ++i) {
        u_int8_t cardId[CARD_ID_SIZE];
        // Populate cardId with appropriate values
        new_card_detected(cardId);
    }

    // Sleep for a while to allow tasks to complete
    sleep(5);

    // Shutdown the thread pool
    clean_up_validation_thread_pool();

    return 0;
}
