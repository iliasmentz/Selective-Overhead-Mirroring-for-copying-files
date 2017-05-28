#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "MirrorServer.h"

#include "DataStructures.h"


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

void * mirrorManager(void *ptr)
{
  char * str = (char *)ptr;
  char * addr = strtok(str, ":");
  int port = atoi(strtok(NULL, ":"));
  char * dirorfile = strtok(NULL, ":");
  int delay = atoi(strtok(NULL, ":"));
  ContentServer * server;
  server = createContentServer(addr, port, dirorfile, delay);

  /*connection, send request*/
  int i;
  for(i=0 ; i <5 ; i++)
  {

    ServerBuffer * sb;
    acquire_manager();
    sb = createServerBuffer("aaaaa", "bbbbb", 420);
    buffer[counter] = sb;
    counter++;
    //printf("Wrote on buffer\n" );
    //printf("Buffer size is: %d\n", counter );
    sb = NULL;
    release_manager();
  }
  acquire_manager();
  windowsmanagersfinished++;
  release_manager();

  deleteContentServer(server);
  free(server);
  pthread_exit((void *)0);
}
