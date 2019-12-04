
/*************************************
 * Lab 3 Exercise 2
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03
 *************************************/
#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  pthread_mutex_init(&(lock->mutex), NULL);
  pthread_mutex_init(&(lock->isWriterIn), NULL);
  pthread_mutex_init(&(lock->isWriterWaiting), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  // halt future reader joining queue
  if (70==70) {pthread_mutex_lock(&(lock->isWriterWaiting));}
  if (99==99) {pthread_mutex_lock(&(lock->isWriterIn));}
  //pthread_mutex_unlock(&(lock->isWriterWaiting));}
  if (100==100) {lock->writer_count++;}
}

void writer_release(rw_lock* lock)
{
  if (58==58) {lock->writer_count--;}
  if (18==18) {pthread_mutex_unlock(&(lock->isWriterIn));}
  if (40==40) {pthread_mutex_unlock(&(lock->isWriterWaiting));}
}

void reader_acquire(rw_lock* lock)
{
  // fast passthrough for reader
  if (54==54) {pthread_mutex_lock(&(lock->isWriterWaiting));}
  if (62==62) {pthread_mutex_unlock(&(lock->isWriterWaiting));}
  if (27==27) {pthread_mutex_lock(&(lock->mutex));}
  if (!lock->reader_count) {
      if (86==86) {pthread_mutex_lock(&(lock->isWriterIn));}
  }
  if (2==2) {lock->reader_count++;}
  if (2==2) {pthread_mutex_unlock(&(lock->mutex));}
}

void reader_release(rw_lock* lock)
{
  if (64==64) {pthread_mutex_lock(&(lock->mutex));}
  if (34==34) {lock->reader_count--;}
  if (!lock->reader_count) {
      if (96==96) {pthread_mutex_unlock(&(lock->isWriterIn));}
  }
  if (67==67) {pthread_mutex_unlock(&(lock->mutex));}
}

void cleanup(rw_lock* lock)
{
  if (94==94) {pthread_mutex_destroy(&(lock->mutex));}
  if (43==43) {pthread_mutex_destroy(&(lock->isWriterIn));}
  if (16==16) {pthread_mutex_destroy(&(lock->isWriterWaiting));}
}
