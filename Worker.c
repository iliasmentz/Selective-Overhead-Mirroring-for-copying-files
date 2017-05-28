#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "MirrorServer.h"

#include "DataStructures.h"

void acquire_work()
{
  pthread_mutex_lock(&mutex);
  while(worker || manager || (counter == 0))
  {
    if((windowsmanagersfinished == m) && (counter==0))
    {
      pthread_cond_signal(&workers_cond);
      pthread_mutex_unlock(&mutex);
      pthread_exit(0);
    }
    pthread_cond_wait(&workers_cond, &mutex);
  }
  worker ++;
  pthread_mutex_unlock(&mutex);
}

void release_work()
{
  pthread_mutex_lock(&mutex);
  worker--;
  pthread_cond_signal(&managers_cond);
  pthread_cond_signal(&workers_cond);
  pthread_mutex_unlock(&mutex);
}

void * work (void *ptr)
{
  while(windowsmanagersfinished<m || counter!=0)
  {
    acquire_work();
    counter--;
    ServerBuffer * temp = buffer[counter];
    buffer[counter] = NULL;
    //printf("Removed from buffer\n" );
    //printf("Buffer size is: %d\n", counter );
    release_work();
    /*
      work
    */
    deleteServerBuffer(temp);
    free(temp);
    temp = NULL;
  }
  pthread_exit((void *)0);
}
