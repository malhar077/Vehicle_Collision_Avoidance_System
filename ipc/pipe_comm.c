#include <stdio.h>
#include <unistd.h>
#include <string.h>

int fd[2];

void init_pipe() {
    pipe(fd);
}

void write_pipe(char *msg) {
    write(fd[1], msg, strlen(msg));
}

void read_pipe() {
    char buffer[100];
    int n = read(fd[0], buffer, sizeof(buffer));
    buffer[n] = '\0';
    printf("📨 Pipe received: %s\n", buffer);
}