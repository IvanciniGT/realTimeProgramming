#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    printf("Starting sender...\n");
    printf("Forking the process...\n");
    pid_t pid = fork();

    if(pid == -1) {
        printf("Error while forking\n");
        return 1;
    }
    if (pid == 0) { // This is the code that will be executed after the fork...
        printf("Launching receiver...\n");
        // The child process will execute this code... the receiver
        // this will be the new receiver process running
        execl("./signals2", "signals2", NULL);
        perror("Problems while launching receiver");
        return 1;
    }else {
        // This is the code that will be executed by the parent process
        printf("This is the parent process\n");
        printf("The pid of the parent process is: %d\n", getpid());
        printf("The pid of the child process is: %d\n", pid);
        printf("let's wait for 5 seconds...\n");
        sleep(5);
        printf("let's send a signal SIGUSR1 to the child process...\n");
        kill(pid, SIGUSR1);
        printf("let's wait for 5 seconds...\n");
        sleep(5);
        printf("let's send a signal SIGUSR2 to the child process...\n");
        kill(pid, SIGUSR2);
        printf("let's wait for 5 seconds...\n");
        sleep(5);
        printf("let's send a signal SIGTERM to the child process...\n");
        kill(pid, SIGTERM);
        printf("let's wait for 5 seconds...\n");
        sleep(5);
        return 0;

    }
}
