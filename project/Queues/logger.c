#include <stdio.h>

#include <mqueue.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

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
    our_queue_attributes.mq_flags = 0;

    // Let's create/open the queue
    our_queue = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR, &our_queue_attributes);
    if (our_queue == -1) {
        perror("Error creating/opening the queue");
        return 1;
    }
    return 0;
}

void close_queue(int signal){
    mq_close(our_queue); // We should keep the exit of this function in a variable and return it at the very end
    mq_unlink(QUEUE_NAME); // Same here
}

int main() {

    // Control sigint
    struct sigaction sigint_action;
    sigint_action.sa_handler = close_queue;
    sigaction(SIGINT, &sigint_action, NULL);

    if(initialize_queue() == 1) {
        perror("Error initializing the queue. Aborting...");
        return 1;
    }
    int counter = 0;
    while(counter < 100) {
        char buffer[MAX_MESSAGE_SIZE];
        printf("Waiting for a message...\n");
        ssize_t bytes_read = mq_receive(our_queue, buffer, MAX_MESSAGE_SIZE, NULL);
        if(bytes_read == -1) {
            perror("Error receiving message from the queue");
        } else {
            printf("Received message: %s\n", buffer);
        }
        counter++;
    }

    // Close the queue
    close_queue(0);
    return 1;

}
