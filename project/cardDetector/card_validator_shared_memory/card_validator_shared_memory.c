#include <semaphore.h>

int shared_memory_fd;
struct SharedData *shared_data;
sem_t *semaphore;
