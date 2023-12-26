#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int getPolicy() {
    return sched_getscheduler(0);
}

int getP() {
    return getpriority(PRIO_PROCESS, 0);
}

void executeTask() {
    long long sum = 0;
    for (long long i = 0; i <= 1500000000; ++i) {
        sum += i;
    }
}

void policy_pretty(int x) {
    switch (x) {
    case SCHED_OTHER:
        printf("normal");
        break;
    case SCHED_FIFO:
        printf("fifo");
        break;
    case SCHED_RR:
        printf("round robin");
        break;
    default:
        printf("uknown");
        break;
    }
}

double elapsed_time(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9; // 1e9 = 1 * 10 στην δύναμη του 9, δηλαδή 1 δισεκατομμύριο
}

int main() {
    int status = EXIT_SUCCESS;
    int child_pid = fork();

    if (child_pid == 0) { // Διεργασία παιδί
        printf("[ Child Process (%d) ]\n", getpid());
        printf("Scheduling Policy: ");
        policy_pretty(getPolicy());
        printf("\n");
        printf("Nice Value: %d\n", getP());

        struct timespec start_time, end_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);

        executeTask();

        clock_gettime(CLOCK_MONOTONIC, &end_time);

        printf("\nTime taken for child process: %f seconds\n", elapsed_time(start_time, end_time));
    } else if (child_pid > 0) { // Διεργασία γονέας
        printf("[ Parent Process (%d) ]\n", getpid());
        printf("Scheduling Policy: ");
        policy_pretty(getPolicy());
        printf("\n");
        printf("Nice Value: %d\n", getP());

        struct timespec start_time, end_time;
        clock_gettime(CLOCK_MONOTONIC, &start_time);

        executeTask();

        clock_gettime(CLOCK_MONOTONIC, &end_time);

        printf("\nTime taken for parent process: %f seconds\n", elapsed_time(start_time, end_time));

        // Αναμονή για την ολοκλήρωση της διεργασίας παιδιού
        if (waitpid(child_pid, &status, 0) != child_pid)  {
  			exit(status);
		}
    } else {
        perror("fork");
        status = EXIT_FAILURE;
    }

    return status;
}