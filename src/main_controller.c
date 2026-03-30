#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include "ipc.h"
#include "common.h"


#define NUM_VEHICLES 3

extern struct VehicleData *shared_data;
extern struct VehicleData* get_shared_memory();


int warning_flag = 0;
int step_after_warning = 0;


void write_log(const char *text) {
  int fd = open("../log/vehicle_log.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);

    if (fd < 0) return;

    char log_entry[200];

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    sprintf(log_entry, "[%02d:%02d:%02d] %s\n",
            t->tm_hour, t->tm_min, t->tm_sec, text);

    write(fd, log_entry, strlen(log_entry));

    close(fd);
}

void handle_stop(int sig) {
    printf("Car stopped due to collision!\n");
    write_log("Car STOPPED due to collision");
    exit(0);
}

// ⏰ SIGALRM
void handle_alarm(int sig) {
    if (warning_flag == 1) {
        step_after_warning++;

        if (step_after_warning >= 2) {
            warning_flag = 2;
        }
    }

    alarm(2);
}

// 📊 Monitor Thread
void *monitor_thread(void *arg) {
    while (1) {
        lock();

        printf("\n--- Vehicle Data ---\n");
        for (int i = 0; i < NUM_VEHICLES; i++) {
            printf("Vehicle %d → position=%d speed=%d\n",
                   shared_data[i].vehicle_id,
                   shared_data[i].position,
                   shared_data[i].speed);
        }

        unlock();
        sleep(2);
    }
}

// 🚨 Collision Thread
void *collision_thread(void *arg) {

    while (1) {
        lock();

        for (int i = 0; i < NUM_VEHICLES; i++) {
            for (int j = i + 1; j < NUM_VEHICLES; j++) {

                int diff = abs(shared_data[i].position - shared_data[j].position);

                // ⚠️ WARNING
                if (diff <= 25 && warning_flag == 0) {
                    printf("\n⚠️ WARNING: Vehicles %d & %d close\n",
                           shared_data[i].vehicle_id,
                           shared_data[j].vehicle_id);

                    write_log("WARNING: Vehicles getting close");

                    printf("🚗 Driver Action: Reducing speed!\n");
                    write_log("ACTION: Speed reduced");

                    shared_data[i].speed -= 10;
                    shared_data[j].speed -= 10;

                    if (shared_data[i].speed < 0) shared_data[i].speed = 0;
                    if (shared_data[j].speed < 0) shared_data[j].speed = 0;

                    warning_flag = 1;
                    step_after_warning = 0;
                }

               
                else if (diff <= 10 && warning_flag == 2) {

                    printf("\n🚨 COLLISION DETECTED (%d & %d)\n",
                           shared_data[i].vehicle_id,
                           shared_data[j].vehicle_id);

                    char log_msg[100];
                    sprintf(log_msg, "COLLISION: Vehicles %d & %d",
                            shared_data[i].vehicle_id,
                            shared_data[j].vehicle_id);

                    write_log(log_msg);

                    send_message("Collision detected via Message Queue");

                    char fifo_msg[100];
                    sprintf(fifo_msg, "FIFO ALERT: Collision between %d & %d\n",
                            shared_data[i].vehicle_id,
                            shared_data[j].vehicle_id);

                    send_fifo(fifo_msg);

                    sleep(1);
                    kill(getpid(), SIGTERM);
                }
            }
        }

        unlock();
        sleep(1);
    }
}

int main() {

    signal(SIGTERM, handle_stop);
    signal(SIGALRM, handle_alarm);

    alarm(2);

    init_shared_memory();
    init_msg_queue();
    init_fifo();
    init_semaphore();

    shared_data = get_shared_memory();


    shared_data[0] = (struct VehicleData){1, 0, 50};
    shared_data[1] = (struct VehicleData){2, 50, 50};
    shared_data[2] = (struct VehicleData){3, 100, 30};

    pthread_t t1, t2;

    pthread_create(&t1, NULL, monitor_thread, NULL);
    pthread_create(&t2, NULL, collision_thread, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}