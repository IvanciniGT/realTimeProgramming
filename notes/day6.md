
Card sensor process

    When ON
        <- detection event ID (uint8_t[]) ---> Card Validation Process

In the training we are just using different OS processes for each component of that machine.
We could also be working with Threads


3 sensors (RFID / magentic / Optical QR ) -> Unique: card Validation Process


Could we use a signal for that to send that information? RT Signals: 32 - 64
We will need to implement a different communication mechanism to send the event ID to the card validation process.

How many card ids are we going to deal with at a time? 1

Can we save the variable in a common register? Shared memory
But... shared memory has a limitation

    If we make use shared memory we may need a mutex to protect the shared memory.
    - We need to make sure that the other process is going to read that new value:
    - Process 1 -> Send a notification -> Process 2 -> Read the value <--- semaphore / mutex / condition variable  -  signal
    - Process 2 could be just checking for that value each X      <---  We probably don't want this 

But the thing with shared memory is that both processes needs to be running in the same machine.

If those processes could be running at certain point in different machines -> RT Sockets


We may have 3 different card detection processes, and 1 card validation process.

                            Card Detection Process  Card Id validationProcess
                                    write             read
                                      v                v
        +--------------------------> Shared memory (CardId) <---------------+
        |                                                                   |
        |                                                                   |
    Card Detection Process                                        Card Id validationProcess
        ^                                                               sem_wait(MY_SEMAPHORE) -> mutex lock (MY_MUTEX) -> read ->  write -> mutex unlock (MY_MUTEX)
        |                                                                             
        |                
      positive_detection mutex lock (MY_MUTEX) -> write -> mutex unlock (MY_MUTEX) -> sem_post(MY_SEMAPHORE)
        |
      driver
        |
        |
    Sensor -> ir 


When working with semaphores, we will have 2 functions: sem_wait() and sem_post()
    sem_wait() will block the process until the semaphore is available.
    sem_post() will release the semaphore.



Card Id validationProcess

    sem_wait(MY_SEMAPHORE)  this is gonna wait for the semaphore to be available. The process is going to be stuck at this point

Do I want this process to be for example sending vital signs to the main system in parallel? YES <- main 
2º Thread wait for incomming cardid
3º Thread for requesting validation

The point is that we need to make use of threads



                      MAIN PROCESS                                      MAIN PROCESS               
                            v                                               v            v restart signal (You don't have to mess with shared memory)
                    start those process                                  shutdown
         v                               v                                       
    Card validator process          Card detection process              both will try to delete the shared memory
         v                               v
        Both will try to create the Shared memory
    