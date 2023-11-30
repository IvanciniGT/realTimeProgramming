#include <stdio.h>

#include <mqueue.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define QUEUE_NAME "/my_queue"
#define MAX_QUEUE_SIZE 10
#define MAX_MESSAGE_SIZE 100

mqd_t our_queue;

void send_message(char buffer[MAX_MESSAGE_SIZE]) ;

int initialize_queue() {

    // Let's configure the queue attributes
    struct mq_attr our_queue_attributes;
    our_queue_attributes.mq_maxmsg = MAX_QUEUE_SIZE;
    our_queue_attributes.mq_msgsize = MAX_MESSAGE_SIZE;
    our_queue_attributes.mq_flags = O_NONBLOCK; //(non-blocking queue)

    // Let's create/open the queue
    our_queue = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR, &our_queue_attributes);
    if (our_queue == -1) {
        perror("Error creating/opening the queue");
        return 1;
    }
    return 0;
}


int close_queue() {
    mq_close(our_queue); // We should keep the exit of this function in a variable and return it at the very end
    return mq_unlink(QUEUE_NAME); // Same here
}

int main() {
    pid_t pid = getpid();
    printf("Sender PID: %d\n", pid);
    if(initialize_queue() == 1) {
        perror("Error initializing the queue. Aborting...");
        return 1;
    }
    // ten times
    for(int i = 0; i < 10; i++) {
        char buffer[MAX_MESSAGE_SIZE];
        // Add the PID to the message
        strcpy(buffer, "Hello I am the sender with PID: ");
        char pid_string[10];
        sprintf(pid_string, "%d", pid);
        strcat(buffer, pid_string);
        send_message(buffer);
        printf(".");
        usleep(2000000);
    }
    return close_queue();
}

void send_message(char buffer[MAX_MESSAGE_SIZE]) {
    // Send a message to the queue
    if (mq_send(our_queue, buffer, strlen(buffer) + 1, 0) == -1) {
        perror("Error sending message to the queue. Probably the queue is full");
    }
}