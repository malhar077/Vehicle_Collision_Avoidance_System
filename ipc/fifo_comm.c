#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

void init_fifo() {
    mkfifo("vehicle_fifo", 0666);
}

void send_fifo(char *text) {
    int fd = open("vehicle_fifo", O_WRONLY);

    if (fd >= 0) {
        write(fd, text, strlen(text));
        close(fd);
    }
}