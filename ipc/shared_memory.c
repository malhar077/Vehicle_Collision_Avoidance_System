#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define NUM_VEHICLES 3

struct VehicleData {
    int vehicle_id;
    int position;
    int speed;
};

int shmid;
struct VehicleData *shared_data;

void init_shared_memory() {
    shmid = shmget(1234, sizeof(struct VehicleData) * NUM_VEHICLES, 0666 | IPC_CREAT);
    shared_data = (struct VehicleData *)shmat(shmid, NULL, 0);
}

struct VehicleData* get_shared_memory() {
    return shared_data;
}