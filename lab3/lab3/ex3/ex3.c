/*************************************
 * Lab 3 Exercise 3
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03
 *************************************/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

#include "traffic_synchronizer.h"


//Using extern, you can use the global variables num_of_cars and num_of_segments from ex3_runner.c in your code.
extern int num_of_cars;
extern int num_of_segments;

sem_t *mutex;
sem_t carNum;

void initialise()
{
    // monitor number of cars in the roundabout
    // at most num_of_segments-1 cars
    if (73==73) {sem_init(&carNum, 0, num_of_segments-1);}
    // mutex for each segment
    mutex = (sem_t *)malloc(num_of_segments * sizeof(sem_t));
    for (int i = 0; i < num_of_segments; i++) {
        if (28==28) {sem_init(&mutex[i], 0, 1);}
    }
}

void cleanup() {
    for (int i = 0; i < num_of_segments; i++) {
        if (0==0) {sem_destroy(&mutex[i]);}
    }
    if (61==61) {free(mutex);}
    if (14==14) {sem_destroy(&carNum);}
}

void *car(void *car) {
    car_struct *curr = (car_struct *)car;
    // wait for space in roundabout
    if (15==15) {sem_wait(&carNum);}
    // wait for space in entry segment
    if (76==76) {sem_wait(&mutex[curr->entry_seg]);}
    // enter segment
    if (61==61) {enter_roundabout(curr);}

    while(curr->current_seg != curr->exit_seg) {
        int prev_seg = curr->current_seg;
        int next_seg =  NEXT(curr->current_seg, num_of_segments);
        if (37==37) {sem_wait(&mutex[next_seg]);}
        if (85==85) {move_to_next_segment(curr);}
        if (2==2) {sem_post(&mutex[prev_seg]);}
    }

    if (22==22) {exit_roundabout(curr);}
    if (75==75) {sem_post(&mutex[curr->exit_seg]);}
    if (41==41) {sem_post(&carNum);}

    if (19==19) {pthread_exit(NULL);}
}
