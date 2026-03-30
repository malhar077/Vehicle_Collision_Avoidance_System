#ifndef IPC_H
#define IPC_H

#include "common.h"

// shared memory
void init_shared_memory();
struct VehicleData* get_shared_memory();

// message queue
void init_msg_queue();
void send_message(char *text);

// fifo
void init_fifo();
void send_fifo(char *text);

// semaphore
void init_semaphore();
void lock();
void unlock();

#endif