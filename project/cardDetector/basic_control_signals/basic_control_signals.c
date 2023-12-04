
#include <stdio.h>
#include <signal.h>
#include "../card_detector/card_detector.h"

int const TURN_ON_SIGNAL= SIGUSR1; //SIGRTMIN + 0;
int const TURN_OFF_SIGNAL= SIGUSR2; //SIGRTMIN + 1;
int const SHUTDOWN_SIGNAL= SIGQUIT;

void turn_on_signal_handler(int signo, siginfo_t *info, void *extra) {
    // We are just using a signal to expose a function to another process
    // But maybe nowadays... or in the future... we could use a socket to expose that function
    // Or maybe we could even use both... a signal and a socket
    // Or we could actually be using this function internally also.
    // In any of those I would do that check... So I prefer to keep thet check in the function (turn_on)
    turn_on();
}

void turn_off_signal_handler(int signo, siginfo_t *info, void *extra) {
    turn_off();
}

void shutdown_signal_handler(int signo, siginfo_t *info, void *extra) {
    // Is to test that that signal comes from the main process
    shutdown_system();
}


void configure_signal_handlers() {
    struct sigaction turn_on_action;
    turn_on_action.sa_sigaction = turn_on_signal_handler;
    turn_on_action.sa_flags = SA_SIGINFO;           // Use the extended signal handler... in order to get the PID of the sender
    sigaction(TURN_ON_SIGNAL, &turn_on_action, NULL);

    struct sigaction turn_off_action;
    turn_off_action.sa_sigaction = turn_off_signal_handler;
    turn_off_action.sa_flags = SA_SIGINFO;           // Use the extended signal handler... in order to get the PID of the sender
    sigaction(TURN_OFF_SIGNAL, &turn_off_action, NULL);

    struct sigaction shutdown_action;
    shutdown_action.sa_sigaction = shutdown_signal_handler;
    shutdown_action.sa_flags = SA_SIGINFO;           // Use the extended signal handler... in order to get the PID of the sender
    sigaction(SHUTDOWN_SIGNAL, &shutdown_action, NULL);
}