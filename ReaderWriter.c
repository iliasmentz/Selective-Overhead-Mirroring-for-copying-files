#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "ContentServer.h"

/*reader - writer thread sychronization for the delaysID*/


void reader_lock()
{
  pthread_mutex_lock(&mutex);
  while(writer)
  {
    pthread_cond_wait(&readers_cond, &mutex);
  }
  reader ++;
  pthread_mutex_unlock(&mutex);
}

void reader_release()
{
  pthread_mutex_lock(&mutex);
  reader--;
  if(reader==0)
    pthread_cond_signal(&writers_cond);
  pthread_mutex_unlock(&mutex);
}

void writer_lock()
{
  pthread_mutex_lock(&mutex);
  while (writer || reader)
  {
    pthread_cond_wait(&writers_cond, &mutex);
  }
  writer++;
  pthread_mutex_unlock(&mutex);
}

void writer_release()
{
  pthread_mutex_lock(&mutex);
  writer--;
  pthread_cond_signal(&writers_cond);
  pthread_cond_broadcast(&readers_cond);
  pthread_mutex_unlock(&mutex);
}
