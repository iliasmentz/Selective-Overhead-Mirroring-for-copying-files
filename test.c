#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "DataStructures.h"

#define BUFFERSIZE 1024

ServerBuffer * buffer[1024];
pthread_mutex_t mutex;
int counter=0;
int worker =0;
int manager =0;
int w =0;
int m =0;
pthread_cond_t managers_cond;
pthread_cond_t workers_cond;
int windowsmanagersfinished=0;

void acquire_work();
void release_work();
void * work (void *ptr);
void acquire_manager();
void release_manager();
void * windowManager(void *ptr);

int main (int argc, char * argv[])
{
  //memset(buffer, '\0', 1024);
  w = atoi(argv[1]);
  m = atoi(argv[2]);
  int i;
  pthread_t worker[w];
  pthread_t manager[m];
  pthread_mutex_init(&mutex, 0);
  pthread_cond_init(&managers_cond, 0);
	pthread_cond_init(&workers_cond, 0);
  for(i=0; i < m; i++)
  {
    pthread_create(&manager[i], 0  , windowManager, 0);
  }
  for(i=0; i < w; i++)
  {
    pthread_create(&worker[i], 0  , work, 0);
  }
  for(i=0; i < m; i++)
  {
    pthread_join(manager[i], 0);
  }
  printf("Managers finished!\n" );
  for(i=0; i < w; i++)
  {
    pthread_join(worker[i], 0);
  }
  printf("Workers finished!\n" );
  printf("Buffer size is: %d\n", counter );
  pthread_cond_destroy(&managers_cond);
  pthread_cond_destroy(&workers_cond);
  pthread_mutex_destroy(&mutex);
}

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
    printf("Removed from buffer\n" );
    printf("Buffer size is: %d\n", counter );
    release_work();
    /*
      work
    */
    deleteServerBuffer(temp);
    free(temp);
    temp = NULL;
  }
  return (void *)0;
}


void acquire_manager()
{
  pthread_mutex_lock(&mutex);
  while (worker || manager || (counter==BUFFERSIZE))
  {
    pthread_cond_wait(&managers_cond, &mutex);
  }
  manager++;
  pthread_mutex_unlock(&mutex);
}

void release_manager()
{
  pthread_mutex_lock(&mutex);
  manager--;
  pthread_cond_signal(&workers_cond);
  pthread_cond_signal(&managers_cond);
  pthread_mutex_unlock(&mutex);
}

void * windowManager(void *ptr)
{
  /*connection, send request*/
  int i;
  for(i=0 ; i <5 ; i++)
  {

    ServerBuffer * sb;
    acquire_manager();
    sb = createServerBuffer("aaaaa", "bbbbb", 420);
    buffer[counter] = sb;
    counter++;
    printf("Wrote on buffer\n" );
    printf("Buffer size is: %d\n", counter );
    sb = NULL;
    release_manager();
  }
  acquire_manager();
  windowsmanagersfinished++;
  release_manager();
  return (void *)0;
}
