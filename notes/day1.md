
# Real time programming

## What is real time programming?

Is the set of techniques and tools that allow us to build systems that are able to respond to events in a timely manner.

Timely manner? What does that mean?
It does not mean that the system is fast, it means that the system is able to respond to events in a predictable manner (deterministic system).
We could also say that the system needs to respond to events in a bounded time => THIS IS A REQUIREMENT.

When we have constrains / requirements regarding time, we need to apply / make use of special techniques and tools to be able to meet those requirements.

### Kinds of real time systems

- Hard real time systems: The system needs to respond to events in a bounded time.
                          If the system does not respond in the bounded time, the system is considered to be failed.
                          Even 1 single event that is not responded in the bounded time, the system is considered to be failed.
                            Examples:
                            - Airplane control system
                            - Heart pacemaker
                            - Lift / elevator control system (stop)
- Firm real time systems: The system needs to respond to events in a bounded time.
                          BUT if the system does not respond in the bounded time, the system is not considered to be failed.
                          The quality of the system is degraded.
                          The later the answer, the worse the quality.... and the later the answer, that answer is considered less useful.
                          We can loose some events, but not all of them... An event lost should happen very rarely.
                          Examples:
                            - Video streaming
                          We could have some delays 
- Soft real time systems: The system needs to respond to events in a bounded time.
                          BUT if the system does not respond in the bounded time, the system is not considered to be failed.
                          The quality of the system is degraded if we loose too many events.
                          Once the response frame is over, the system is not interested in that event anymore.
                          Example: 
                            - Video streaming
                             Guy A - Frame1, Frame 2, Frame 3 ---> Guy B
                             In case Frame 2 was not processed in a bounded time, the system is not interested in that frame anymore.
                          We could have some losses.

- Video game... Imagine a video game that is played online...
  - And a player makes a movement... 
    Chess game... If the movement is not responded in a bounded time, the game is not failed.
    Shooter game (Counter Strike)... If the movement is not responded in a bounded time, the game is failed.
    Shooter game (Counter Strike)... If the movement is not responded in a bounded time, the game is not failed, but the quality is degraded.It could not happen that I shoot to a player and the player does not die. If this happens... the game is failed: That process is a hard real time process.
- For example... In that video game... Imagine I am not able to render the screen in a bounded time... and the screen freezes... The game is not failed, but the quality is degraded. That process is a firm real time process.

This is hard. In order to achieve this, we need to apply special techniques and tools.

## Concepts related to real time programming

- Event: Something that happens in the system/environment at a given time T.
- Time Frame: A period of time that starts at a given time T and ends at a given time T+(time frame duration).
  Usually, restrictions are applied to the time frame duration. 
  Depending on teh system nature, the time frame duration could be milliseconds, microseconds, nanoseconds, etc.
- Response time: The time that the system needs to respond to an event.
  This response time, consist of 2 parts:
    - Processing time: The time that the system needs to process the event/ to fulfill the response.
                       Is usually deterministic. I can know how much time the system needs to process the event. To produce the response. 
    - Waiting time: The time that the system was waiting wait until it started to process the event. <<<  This right here is the problematic part.
                      This part is usually non-deterministic. I do not know how much time the system will need to wait until it starts to process the event. We will need to estimate it... and to apply a generous margin.
                    <---------------------TIME FRAME------------------------> *** Constraint
                   T   waiting time   processing time
                   v<---------------><----------------->
    ---------------x·················OOOOOOOOOOOOOOOOOOX---------------------|------------------------> TIME
                   ^             ^                     ^
                   event                                IS WHEN THE SYSTEM RESPONDS TO THE EVENT
                   v
                   SYSTEM 

The problem is that the system is going to be doing a bunch of things "at the same time"... and we do not know when the event is going to happen in most of the cases.
- Some of those thing... we have control over them... and ... somehow we will be able to assign priorities to them.
- But some of those things... we do not have control over them... and we have to make sure that they do not interfere with the things that we have control over.... and priority.
  
---
Example of thing we do not control:
- Programming Language Features
  - Python

    ```py
    text = "Hello"          (line 1)
    text = "Bye"            (line 2)
    ```
    What line 1 does?
    - First thing: It creates an "str" object with value "Hello" and places it in memory.
    - Second thing: It creates a variable called "text"
    - Finally:      It assigns the variable to the object. Not the objeto to the variable... but the opposite.
                    Kind a pointer to the object in C++.
    What line 2 does?
    - First thing: It creates an "str" object with value "Bye" and places it in memory.
                    The question here is... where in memory?
                    Same place as "Hello"? or a different place? In a different place.
                    So... At this time we have 2 different objects in memory: "Hello" and "Bye".
    - Second thing: It gets the variable called "text" 
    - Finally:      It reassigns that variable to the new object created.
                    At this moment, the str object "Hello" becomes: garbage.
                    Why? There is no way in python to access that object anymore.
                    If this was C++... we could have a pointer to that object... 
                    or we could calculate the address of that object... and we could access it.
                    But in python... we do not have that possibility... at all. Same in JAVA, JS.

                    Automatically, Python marks them as garbage... at certain point... they will be removed (or not) 
                    from the memory.
                    Python, same as JAVA, JS, etc... has a garbage collector.

                    This right here is a python....

Regarding real time programming... this is a problem... because we do not know when the garbage collector is going to be executed... and we don't know how much time it is going to take to execute it. We have no control over it.
That why we don't use python, nor JAVA for real time programming.
- On top of out hardware... we will install an Operating system (or maybe not)
    - Windows, or MacOS... or some Linux distributions (such as Ubuntu, Debian, RedHat, etc)
        Those OS are not real time OS, because we do not have control over them... in different ways:
        - We cannot control when an OS background process is going to be executed... such as the the automatic update process... or a swap process... or a process that is going to be executed when we connect a USB device.
        - And also, the OS is not going to assure us that a signal is going to be produced at a certain time.
          - Let's configure a timer to wake up/start a process at a certain Time... These OS does not assure us that the process is going to be started at that time.
    - We do have OS that bring us control... and deterministic behavior to solve this situations: Linux RT
- We have hardware. Hardware... does have a deterministic behavior? Kind of...
  - I mean... we cannot make sure that a signal is going to be produced at a certain time... but we can make sure that a signal is going to be produced in a certain time frame. It is usually a very small time frame... and it is usually ok for us... but it is not deterministic.... and we cannot control it... and depending on the requirements of our system... it could be a problem.... and we need to take this into consideration.

---

# How are we going to design a real time system?

- We will have a set of events that are going to be produced by the environment or the system itself.
  We will need to capture/produce those events.

- We will have a set of tasks that are going to be executed in the system, as a response to those events.
  We will need to execute those tasks as soon as possible... in a bounded time after the event is produced.

We need to communicate the events with the tasks.
In other words, we need to send information from the events to the tasks.

Let's talk about those communications.
In general, communications can be classified in 2 types:
- Sync communications: The sender waits until the receiver receives the message.
  Example in the real world: A phone call. I make a call... and I have to be there waiting until the other person picks up the phone.... and answers the call. I cannot be doing other things in the meantime.
  In addition, if the receiver does not pick up the phone... the communication get lost.
  I was not able to deliver the message.
- Async communications: The sender does not wait until the receiver receives the message.
  Example in the real world: I send a whatsapp message. I do not have to wait until the other person reads the message. That means that: 
  - I can do other things in the meantime.
  - In case the receiver is busy... the message is not lost. The message is stored in the receiver phone... and it will be processed as soon as the receiver is available to do it.

In RTP, we will use async communications.
The problem with sync communications is that the sender gets locked until the receiver receives and process the message.
That could be an issue in a real time system... because we do not know how much time the receiver is going to take to receive and process the message... and we may need to produce/receive additional events in the meantime.
The system/component/process firing the event, needs to continue working... and it cannot be blocked.

The thing here, is that we will have a process/thread in a system that is going to be producing events... 
and we will have a set of processes/threads that are going to be consuming those events.

AND WE WILL NEED TO COMMUNICATE THEM EACH OTHER.... We need to send information from the event producer to the event consumer.

---
# Processes and threads

In a computer, OS manages processes and threads.
Each time a software is executed, the OS creates a process to execute that software.
The first thing a process requires is memory:
- Code: Where the code of the software is going to be placed.
- Working area: Where the variables of the software are going to be placed.
  Once a Job is done, the OS frees the memory used by that job (those variables)
- Caching objects: To store objects that are going to be used by the software frequently.
  They are stored in memory, to be able to access them faster.
  We need to take care of that memory... and we need to free it when we do not need it anymore.... or when we need to use that space for something else.
- Buffers: To store temporary data, before sending it to the hardware.
  For example, when we are reading a file from the disk... we read it in chunks... and we store those chunks in a buffer... and then we send them to the hardware.
- Thread Stack: Is used to store information related to each thread the process requires.

The actual work is going to be executed by a thread.

## Threads

A process can have 1 or more threads.
A thread is what the OS uses to execute the code of the software.
A process that opens multiple threads, is able to execute multiple tasks "at the same time".
A system running multiple processes, is also able to execute multiple tasks "at the same time"... as each process is going to be executed by a different thread.

### At the same time

In case we have a 1 core CPU, the OS is only going to be able to execute the threads in a sequential way.
In case we want to truly execute multiple threads at the same time, we need to have a CPU with multiple cores.
When we only have 1 core... we can do a little trick to make it look like we are executing multiple threads at the same time: Time slicing or shared time.

            11111111111111111            Thead 1
            2222222222                   Thead 2

        CPU 111111111111111112222222222     OPTION 1: Sequential execution
        CPU 111112222211111222221111111     OPTION 2: Time slicing or shared time

            * Each 1 or 2 is a task that needs to be executed by the thread.

Event producers and event consumers (which are who process the events) are going to be executed in different threads (belonging to the same process or to different processes).... Its gonna be me (the software developer) who is going to decide that... in most cases. Sometimes, the OS is going to decide that for me, or I am going to have some restrictions due to the hardware I am using.

Anyways, I am going to have 2 threads. And we will need to communicate them each other.

---
We need to find / learn which strategies we can use to communicate threads each other:

- Mutex: We use a mutex when we have at least 2 threads that want to access the same resource.
  In that scenario the mutex acts locking the resource... so that only 1 thread can access the resource at a time.

  For example, I can have 1 thread trying to write a value in memory... and another thread trying to read that value from memory. The mutex allows us to make sure that only 1 thread is accessing the memory at a time.
  If a thread is accessing the memory, the other thread is going to be blocked until the first thread finishes accessing the memory.

    Imagine an event consist of 2 different informations: A and B.
    If we start writing A... and the we start reading at the same time the information from the event from another thread... we could read A and B... but B may not have been updated yet... that could be a problem.
    To solve this situation we create a mutex, that allows to create a lock so that only 1 thread can access the event information at a time.
    1 thread (the producer) could get the lock... then write A and B (as an atomic operation) ... and then release the lock, so that the other thread (the consumer) can get the lock... and read A and B (as an atomic operation).
  
  But this technique has a limitation: It only works when those threads are running in the same process.

- Shared memory: We define to the OS that a region of the RAM memory is gonna be available for 2 different processes.
    We can specify that 1 process has read access to that region, while the other one has write access to that region.
    This is as using a board in the fridge, to leave messages to the other people in the house.
    We could have additional troubles:
    - 1 people /process writing a message... and another one reading the message at the same time.... we will need to implement something (like a mutex) to solve this situation.

- Queue: Is a data structure... that follows a FIFO pattern (First In - First Out)
  I can have a thread sending messages to the queue... and another thread consuming messages from the queue.
  The consumer is going to consume the messages in the same order they were produced.

  Actually, we have 2 different patterns here:
    
        Producer -> Queue 

  - Pull pattern: The consumer is going to be pulling messages from the queue.

        Queue <- Consumer (hey, do you have a message for me?)

  - Push pattern: The queue is going to be pushing messages to the consumer.

        Queue -> Consumer (hey, I have a message for you)

- Sockets: We can use sockets to communicate processes that are running in different computers.
  We can use sockets to communicate processes that are running in the same computer... but if we can make sure that the processes are going to be in the same computer (ALWAYS)... we can make use of other techniques that are more efficient.

---

But... all this is just standard programming... we can use this techniques in any kind of software... not only in real time software.
The point is that in RTP we:
- First: need to make use of those techniques, as we will have multiple threads running in the system, that needs to communicate each other.
- Second: we will need to apply some additional techniques to make sure that the system is going to be able to respond to events in a bounded time.

---

What kind of techniques are we going to apply to make sure that the system is going to be able to respond to events in a bounded time?
- We need to be able to assign PRIORITIES to the tasks that are going to be executed in the system.

Each task is going to be executed in a thread... and each task or thread is going to have a priority.
So that, the OS is going to be able to decide which thread is going to be executed first... and which thread is going to be executed later.

Some programming languages allows us to assign priorities to threads... and some others do not.

But... some times... we need to assign priorities to tasks that are going to be executed in the same thread.
And that thread needs to decide which task is going to be executed first... and which task is going to be executed later.
We need to be able to assign priorities to tasks. We will have different techniques to do that:
- Priority queue: We can use a priority queue to store the tasks... and the queue is going to be in charge of deciding which task is going to be executed first... and which task is going to be executed later.
    This kind of queues does not follow a FIFO pattern... they follow a priority pattern.
- Multiple queues: where each queue is going to have a different priority.
    We can have a queue for high priority tasks... and another queue for low priority tasks.
    The thread is going to be in charge of deciding which queue is going to be consumed first... and which queue is going to be consumed later.

---

When working with multiple threads we need to be aware of different situations:
- Deadlock: When 2 threads are waiting for each other to release a resource.
  For example, 1 thread is waiting for a mutex to be released by the other thread... and the other thread is waiting for the mutex to be released by the first thread.
  In this situation, both threads are going to be blocked forever. -> System is going to be failed.
- Priority inversion: When a high priority task is waiting for a low priority task to release a resource.
When a high priority task is waiting for a resource that is locked by a low priority task.
  For example, 1 thread is waiting for a mutex to be released by another thread... and the other thread has a lower priority.
  In this situation, the high priority thread is going to be blocked until the low priority thread releases the mutex.
  This is not a problem if the low priority thread releases the mutex in a bounded time... but if it does not... the system is going to be failed.

---

The first thing we have to do when designing a real time system is:
- to identify the events that are going to be produced by the environment or the system itself.
- to define the tasks that are going to be executed in the system, as a response to those events.
- to specify the kind of real time process involved in each task.
  - Hard real time process
  - Firm real time process
  - Soft real time process

Then, we will need to identify the communications between the events and the tasks
and choose an appropriate communication technique for each communication.

We will need to specify the priority of each task... and/or the priority of each thread.

And make sure that each task is going to be executed in a bounded time. <<< CONSTRAINT