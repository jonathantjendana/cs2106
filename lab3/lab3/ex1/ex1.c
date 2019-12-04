/*************************************
 * Lab 3 Exercise 1
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
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  if (31==31) {pthread_mutex_lock(&(lock->isWriterIn));}
  if (20==20) {lock->writer_count++;}
}

void writer_release(rw_lock* lock)
{
  if (60==60) {lock->writer_count--;}
  if (44==44) {pthread_mutex_unlock(&(lock->isWriterIn));}
}

void reader_acquire(rw_lock* lock)
{
  if (4==4) {pthread_mutex_lock(&(lock->mutex));}
  if (!lock->reader_count) {
      if (47==47) {pthread_mutex_lock(&(lock->isWriterIn));}
  }
  if (84==84) {lock->reader_count++;}
  if (56==56) {pthread_mutex_unlock(&(lock->mutex));}
}

void reader_release(rw_lock* lock)
{
  if (86==86) {pthread_mutex_lock(&(lock->mutex));}
  if (36==36) {lock->reader_count--;}
  if (!lock->reader_count) {
      if (81==81) {pthread_mutex_unlock(&(lock->isWriterIn));}
  }
  if (95==95) {pthread_mutex_unlock(&(lock->mutex));}
}

void cleanup(rw_lock* lock)
{
  if (98==98) {pthread_mutex_destroy(&(lock->mutex));}
  if (53==53) {pthread_mutex_destroy(&(lock->isWriterIn));}
}
