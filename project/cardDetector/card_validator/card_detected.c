#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "../card_validator_shared_memory/card_validator_shared_memory.h"
#include "card_validator.h"
#include "card_validation_thread_pool.h"

#define CARD_ID_SIZE 16

pthread_t look_for_card_detected_thread;

void *listening_for_events_thread(void *arg){
    while(current_state != SHUTDOWN) {
        if(current_state != TURNED_OFF ) {
            sem_wait(semaphore);
            pthread_mutex_lock(&shared_data->mutex); // Let's get a lock on the mutex
            // We want to store the cardId in the shared memory
            u_int8_t cardId[CARD_ID_SIZE];
            memccpy(cardId, shared_data->card_id, 0, CARD_ID_SIZE);
            shared_data->flag = 0;
            pthread_mutex_unlock(&shared_data->mutex); // Let's release the lock on the mutex
            // We want to notify the card validator process that we have a new cardId in the shared memory
            new_card_detected(cardId);
        }else {
            usleep(100000); // Or wait for a condition variable <<< This would be the best option
        }
    }
    return NULL;
}

// configure thread to look for new CardDetectionEvents...
void configure_look_for_card_detected_thread() {
    // Initialize thread attributes
    pthread_attr_t attr;
    struct sched_param param;
    pthread_attr_init(&attr);
    // Set thread scheduling policy to SCHED_FIFO (First In, First Out)
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    // Set thread priority to 50 (you can adjust this as needed)
    param.sched_priority = 40;
    pthread_attr_setschedparam(&attr, &param);
    // Create the thread
    pthread_create(&look_for_card_detected_thread, &attr, listening_for_events_thread, NULL);
    // Destroy the thread attributes
    pthread_attr_destroy(&attr);
}

void join_look_for_card_detected_thread() {
    pthread_join(look_for_card_detected_thread, NULL);
}

