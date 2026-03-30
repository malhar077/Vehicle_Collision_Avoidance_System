#include <stdio.h>
#include <semaphore.h>

sem_t sem;

void init_semaphore() {
    sem_init(&sem, 0, 1);
}

void lock() {
    sem_wait(&sem);
}

void unlock() {
    sem_post(&sem);
}