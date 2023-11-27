#include <stdio.h>
#include <unistd.h>

int main() {

    printf("Starting the parent process...\n");
    pid_t parent_pid = getpid();

    printf("My process id is: %d\n", getpid());

    pid_t pid = fork();
    // At this point we will have 2 copies of this program in execution
    // Each one running in a different process
    // Each copy is gonna have a different value for pid
    // The parent process (The original copy) will have the pid of the child process
    // The child process will have the pid 0

    if (pid == -1) {
        printf("Error while forking\n");
        return 1;
    }
    if (pid == 0) {
        // This is the code that will be executed by the child process
        printf("This is the child process\n");
        printf("The pid of the child process is: %d\n", getpid());
        printf("The pid of the parent process is: %d\n", parent_pid);
    }
    else {
        // This is the code that will be executed by the parent process
        printf("This is the parent process\n");
        printf("The pid of the parent process is: %d\n", getpid());
        printf("The pid of the child process is: %d\n", pid);
    }
    return 0;
}