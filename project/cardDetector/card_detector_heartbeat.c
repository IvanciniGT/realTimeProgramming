
#include <sys/signal.h>
#include <signal.h>
#include "card_detector_heartbeat.h"

int const HEARTBEAT_SIGNAL = SIGUSR1;// SIGRTMAX;

void heartbeat (pid_t pid){
    kill(pid, SIGUSR1);
}