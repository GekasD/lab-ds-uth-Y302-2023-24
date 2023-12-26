#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

	char *arg[] = { "fileprop", 0 };

    printf("Parent process (%d) is waiting for the child to complete...\n", getpid());

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Child
        execv(arg[0], arg);
    } else { // Parent
		int status;
        if (waitpid(pid, &status, 0) != pid)  {
  			exit(status);
		}

        printf("Parent ends\n");
    }

    return 0;
}