#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    pid_t pid;

    printf(" Vehicle Creator Started (PID=%d)\n", getpid());

    for (int i = 1; i <= 4; i++) {
        pid = fork();

        if (pid == 0) {
            char id[10];
            sprintf(id, "%d", i);

            execl("./vehicle_process", "vehicle_process", id, NULL);

            perror("exec failed");
            exit(1);
        }
    }

    printf(" All vehicles created successfully!\n");

    return 0;
}