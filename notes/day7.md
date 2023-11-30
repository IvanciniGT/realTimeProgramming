

                            MAIN SYSTEM                                                                        CENTRAL SERVER
                                ^                                                                                              
Signals:     v turn on/off               ^ heartbeats                                                            validateCardTickets
        v                                       v                              < Sockets >                              Monitor the system... this one               
    Card validator process  < Shared memory >  Card detection process                                                   and a bunch of systems like this

All these process are doing a bunch of things


    Having an external process in charge of all those communications is a good idea

                    Central server
                        ^
                      Socket
                        ^
                    Logger Process                              consume                    mq_receive
        ^               ^               ^   
 CardValidatorProcess  MainProcess  CardDetectionProcess        queue a new Message        mq_send /Is a blocking call? Should be non blocking


 Sockets communication  vs  shared memory ... 
  (TCP/UDP)

  Queues


  If queue is full, mq_send will block until there is space in the queue.... Actually this is the default behaviour.
  We can change this behaviour by setting the O_NONBLOCK flag in the mq_open function.
  If we configure the queue as non blocking, then mq_send will return an error if the queue is full. Actually this is not a problem... We will just receive 1.
  The problem is that we are loosing information. We are not able to process all the events.

  If there is no message in the queue, mq_receive will block until there is a message in the queue.... Actually this is the default behaviour.
  We can change this behaviour by setting the O_NONBLOCK flag in the mq_open function.


    Case we are using a non-blocking queue:
    while(1){
        mq_receive
        if (mq_receive == -1){
            if (errno == EAGAIN){
                // No message in the queue
            }
        }else{
            // We have a message in the queue
        }
        nanosleep() // This could introduce a delay in collecting the events... and that could make us loose events.
    }


    Case we are using a blocking queue:
    while(1){
        mq_receive  // This could be a better option.. as we will introduce a smaller delay in collecting the events.
        if (mq_receive == -1){
            if (errno == EAGAIN){
                // No message in the queue
            }
        }else{
            // We have a message in the queue
            // We will send that message to the central server by using a thread from the thread pool. That thread pool is going to have a limited number of threads.
            // Case no thread is available?
                // We could wait until a thread is available.
                    // We are not loosing this message...
                    // But maybe we are loosing other messages, that are not gonna be able to be processed because the queue is gonna be full.
                // Or we could just drop the message.
                    // We are loosing this message...
                    // But we are not blocking the process... and we are able to consume other messages from the queue... more recent messages.
        }
    }
    How long could it take to process a message? 100ms... To communicate with the central server we need 100ms...
    We will do an http post request to the central server... new information is available... and we will send that information to the central server.
    Its ok to receive that information with a 5s delay... It is not a critical information... we can afford to wait 5s to have that information in the central server.
    Here... in this monitor process... we will send that request... and we will waiting for the response... case the response is not received in 5s... we will try to send the request again (maybe 3 times... and then we will give up)
    We will block a thread for a huge amount of time (socket)

    Is it ok to send the request from the same thread we are reading frm the queue? NO
    Is it ok to open a new thread for each request? NO... that's too heavy

    What can we do? We will need to implement a thread pool... and we will need to use a thread from that pool to send the request to the central server.

    We will try to do our best to design the system in a way we have enough threads in the thread pool and space in the queue to process all the events.

    But at any point we may have a problem... and we have to think how to deal with that problem.



---

# Logger

## State Machine:

- TURNED_ON
- TURNED_OFF
- SHUTDOWN
- FAILURE

## Transitions:

- TURN_ON : TURNED_ON -> TURNED_OFF
- TURN_OFF : TURNED_OFF -> TURNED_ON
- SHUTDOWN : TURNED_OFF -> SHUTDOWN
- SHUTDOWN : TURNED_ON -> SHUTDOWN
- FAILURE : TURNED_ON -> FAILURE
- FAILURE : TURNED_OFF -> FAILURE

## Actions:

int initialize() {
    // Create the queue
    // Create the thread pool
    // Open a thread to read from the queue and schedule a task in the thread pool... case there is any available thread.
    // We will set the status to TURNED_OFF
}

int turn_on() {
    // We will set the status to TURNED_ON
}

int turn_off() {
    // We will set the status to TURNED_OFF
}

int shutdown() { // Clean up
    // We will set the status to SHUTDOWN
}

finalCleanUp() {
    // Destroy the thread pool
        // We will try to cancel all the threads in the thread pool
        // We will try to join all the threads in the thread pool
    // Close the queue
    // Unlink the queue
}

int failure() {
    // We will set the status to FAILURE
}

main() {
    initialize();
    while(status != SHUTDOWN) {
        // Send heartbeat
        // wait
    }
    finalCleanUp();
}

----

Listening for signals:

TURN_ON_SIGNAL -> turn_on()
TURN_OFF_SIGNAL -> turn_off()
SHUTDOWN_SIGNAL -> shutdown()

---

Thread consuming the queue:

    while(status == SHUTDOWN) {
        mq_receive // (blocking queue)
        // We have a message in the queue
        // We will check if there is any available thread in the thread pool
        // Case there is any available thread in the thread pool, we will schedule a task in that thread.
        // Case we have no available thread in the thread pool, we will drop the message.

        // Case the system is not TURN_ON, we want to wait for the system to be TURN_ON
    }

---

Each Thread in the pool:

    // struct task {
        message
        thread_id
        retry_count
    }

    // Timeout mechanism... If a thread is not released in 5s... we will make the thread to cancel its current task (We will retry this several times) reschedule the task in the thread pool. (with a limit of retries)

    while(status != SHUTDOWN) {
        // Where we are going to process the message (sending the request to the central server)

        // Case the system is not TURN_ON, we want to wait for the system to be TURN_ON
    }

We want a bunch of thread to wait till the system is TURN_ON


We will have a bunch of threads in this process. Question... All of them should have the same priority?

    3 kind of threads
    - Thread in charge of reading from the queue <<< Medium priority
    - Threads in charge of sending the request to the central server <<< Lower priority
    - Main thread <<<  Higher priority