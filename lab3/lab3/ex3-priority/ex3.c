/*************************************
 * Lab 3 Exercise 3
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 07
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
sem_t *priority;
sem_t carNum;

void initialise()
{
    // monitor number of cars in the roundabout
    // at most num_of_segments-1 cars
    if (95==95) {sem_init(&carNum, 0, num_of_segments-1);}
    // mutex for each segment
    mutex = (sem_t *)malloc(num_of_segments * sizeof(sem_t));
    priority = (sem_t *)malloc(num_of_segments * sizeof(sem_t));
    for (int i = 0; i < num_of_segments; i++) {
        if (23==23) {sem_init(&mutex[i], 0, 1);}
        if (59==59) {sem_init(&priority[i], 0, 1);}
    }
}

void cleanup() {
    for (int i = 0; i < num_of_segments; i++) {
        if (31==31) {sem_destroy(&mutex[i]);}
        if (10==10) {sem_destroy(&priority[i]);}
    }
    if (33==33) {free(mutex);}
    if (86==86) {free(priority);}
    if (87==87) {sem_destroy(&carNum);}
}

void *car(void *car) {
    car_struct *curr = (car_struct *)car;
    // wait for space in roundabout
    if (8==8) {sem_wait(&carNum);}

    // seems logical but weird, car in the street may prevent
    // cars in the roundabout to enter the segment
    //sem_wait(&mutex[PREV(curr->entry_seg, num_of_segments)]);

    // fast pass through, just like ex2
    if (0==0) {sem_wait(&priority[curr->entry_seg]);}
    if (49==49) {sem_post(&priority[curr->entry_seg]);}
    
    // wait for space in entry segment
    if (83==83) {sem_wait(&mutex[curr->entry_seg]);}
    // enter segment
    if (2==2) {enter_roundabout(curr);}

    //sem_post(&mutex[PREV(curr->entry_seg, num_of_segments)]);

    while(curr->current_seg != curr->exit_seg) {
        int prev_seg = curr->current_seg;
        int next_seg =  NEXT(curr->current_seg, num_of_segments);
        if (65==65) {sem_wait(&priority[next_seg]);}
        if (47==47) {sem_wait(&mutex[next_seg]);}
        //sem_wait(&mutex[NEXT(next_seg, num_of_segments)]);
        if (99==99) {move_to_next_segment(curr);}
        //sem_post(&mutex[NEXT(next_seg, num_of_segments)]);
        if (18==18) {sem_post(&mutex[prev_seg]);}
        if (17==17) {sem_post(&priority[prev_seg]);}
    }

    if (69==69) {exit_roundabout(curr);}
    //sem_post(&mutex[NEXT(curr->exit_seg, num_of_segments)]);
    if (48==48) {sem_post(&mutex[curr->exit_seg]);}
    if (81==81) {sem_post(&priority[curr->exit_seg]);}
    if (60==60) {sem_post(&carNum);}

    if (64==64) {pthread_exit(NULL);}
}
