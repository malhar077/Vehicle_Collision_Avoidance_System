#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NUM_VEHICLES 3

struct VehicleData {
    int vehicle_id;
    int position;
    int speed;
};

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("No vehicle ID!\n");
        return 1;
    }

    int id = atoi(argv[1]);

    int shmid = shmget(1234, sizeof(struct VehicleData) * NUM_VEHICLES, 0666);
    struct VehicleData *data = (struct VehicleData *)shmat(shmid, NULL, 0);

    printf(" Vehicle %d started (PID=%d)\n", id, getpid());

   while (1) {

    if (id == 1) {
        data[id-1].position += data[id-1].speed / 10;   // use speed
    }
    else if (id == 2) {
        data[id-1].position -= data[id-1].speed / 10;   // opposite direction
    }
    else {
        data[id-1].position += data[id-1].speed / 20;
    }

    sleep(1);
}

    return 0;
}