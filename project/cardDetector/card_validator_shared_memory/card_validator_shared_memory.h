//
// Created by Ivan Osuna Ayuste on 4/12/23.
//

#ifndef CARDVALIDATOR_CARD_VALIDATOR_SHARED_MEMORY_H
#define CARDVALIDATOR_CARD_VALIDATOR_SHARED_MEMORY_H

#include <pthread.h>
#include <sys/types.h>
#include <semaphore.h>

#define CARD_ID_SIZE 16

struct SharedData {
    u_int8_t card_id[CARD_ID_SIZE];
    int flag;
    pthread_mutex_t mutex;
};

extern int shared_memory_fd;
extern struct SharedData *shared_data;
extern sem_t *semaphore;

#endif //CARDVALIDATOR_CARD_VALIDATOR_SHARED_MEMORY_H
