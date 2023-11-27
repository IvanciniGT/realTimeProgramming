// This file is going to contain a process who will listen for specific signals

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo, siginfo_t *info, void *extra) {
    printf("Signal received: %d from process %d\n", signo, info->si_pid);
}

int main() {
    printf("Starting receiver...\n");
    struct sigaction action;
    action.sa_sigaction = sig_handler;
    action.sa_flags = SA_SIGINFO;           // Use the extended signal handler... in order to get the PID of the sender
    sigaction(SIGUSR1, &action, NULL);
    sigaction(SIGUSR2, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGINT, &action, NULL);
    while (1) {
        sleep(1);
    }
    return 0;
}