#ifndef RIVER_H
#define RIVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// time.h for handling waitting in random time interval:
#include <time.h>
// Libraries for work with semaphores and shared memory:
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

typedef struct{
    int p;
    int h;
    int s;
    int r;
    int w;
    int c;
}param;

#define SERF 0
#define HACKER 1 

param* parse_arguments(int argc, char **argv);
int init_sem(int* [], int []);
int close_sem(int* [], int []);
int create_subprocess(FILE *file, param *par, int *sharedM[], int category);
int create_child(FILE *file, param *par, int category, int number, int *counter[]);
int counter(int *x, int y);
void board(int *seat, sem_t *sem_target);


#endif