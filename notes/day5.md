# Real time signals

POSIX defines signals as a way to communicate with a process.
It defines a bunch of signals:
- SIGINT: interrupt 
- SIGQUIT: quit from keyboard
- SIGKILL: kill signal
- SIGTERM: termination signal
- SIGSTOP: stop the process
- SIGCONT: continue the process if stopped
- SIGCHLD: child process terminated or stopped
- SIGSEGV: invalid memory reference
- SIGPIPE: broken pipe
- SIGALRM: alarm clock
- SIGUSR1: user-defined signal 1
- SIGUSR2: user-defined signal 2

It also defines... as an extension: Real time signals.
Real time signals starts at value: SIGRTMIN (34) and ends at SIGRTMAX (64).
We can send up to 32 different real time signals.

If a process receives real time signals to process events,
- if that process hav less that 32 events, it can use each signal to process each event.
     - SIGRTMIN + 0 -> TURN_ON event
     - SIGRTMIN + 1 -> TURN_OFF event
     - SIGRTMIN + 2 -> POSITIVE detection event
     - SIGRTMIN + 3 -> NEGATIVE detection event
- if that process have more than 32 events.... we need an additional way to supply the event we want to execute... for example by using shared memory.... or by using a pipe.

RT Signals have some differences with normal signals:
- RT signals are queued, normal signals are not. What that means?
    If a process sends a RT signal several times, before the signal is actually delivered, then only the last signal is delivered.
- RD Signals have priority, normal signals do not. 
    SIGTRMIN + 0 has higher priority than SIGTRMIN + 1.
- Not every single POSIX system supports RT signals... for example Mac OS X does not support RT signals.

Normal signals are not queued, so if a process sends a signal several times, then the signal is delivered several times. Each time a signal is send, the OS send that to the process, and the process executes the signal handler. That could be a delayed process... but it will happen.

process --- emit(signal) ---> OS ---- deliver(signal)----> process (signal handler)
                                          QUEUE (always)
                               QUEUE (RT signals)

Non real time signals work like this: 
       SIGUSR1 (S1)-------------------------> S1
       SIGUSR2 (S2)-------------------------> S2 > S1
       SIGUSR1 (S3)-------------------------> S3 > S2 > S1
