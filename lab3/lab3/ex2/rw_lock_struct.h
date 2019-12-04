/*************************************
 * Lab 3 Exercise 2
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03
 *************************************/


#include <pthread.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_mutex_t isWriterIn;
  pthread_mutex_t isWriterWaiting;
  int reader_count;
  int writer_count;
} rw_lock;
