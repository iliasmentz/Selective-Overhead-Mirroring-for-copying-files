#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "DataStructures.h"

#define BUFFERSIZE 1024

void read_args(int argc, char * argv[], int * port, char ** dirname, int * threadnum);

ServerBuffer * buffer[BUFFERSIZE];
int counter=0;
pthread_mutex_t mutex;
int worker =0;
int manager =0;
pthread_cond_t workers;
pthread_cond_t managers;
int windowsmanagersfinished=0;

int main(int argc, char * argv[])
{
  int port=-1;
  char * dirname=NULL;
  int threadnum =-1;

  read_args(argc, argv, &port, &dirname, &threadnum);

  pthread_t worker[threadnum];
  int i;
  for (i = 0 ; i < threadnum;  i++)
  {
    pthread_create(&worker[i], 0, work, (void *)i);
  }
  printf("%s \n", dirname );
  free(dirname);
  exit(EXIT_SUCCESS);
}

void read_args(int argc, char * argv[], int * port, char ** dirname, int * threadnum)
{
  int i;

  for (i = 1; i < argc; i+=2)
  {
    if(strcmp(argv[i], "-p") == 0)
    {
      *port =atoi(argv[i+1]);
    }
    else if(strcmp(argv[i], "-m") == 0)
    {
      *dirname = malloc((strlen(argv[i+1])+1)*sizeof(char));
      strcpy(*dirname, argv[i+1]);
    }
    else if(strcmp(argv[i], "-w")==0)
    {
      *threadnum =atoi(argv[i+1]);
    }
  }

  if((*port < 0) || (*dirname == NULL) || (*threadnum < 0))
  {
    printf("Usage: ./MirrorServer -p <port> -m <dirname> -w <threadnum>\n");
    exit(EXIT_FAILURE);
  }
}

acquire_work()
{
  lock(mutex);
  while(worker || manager || (counter == 0))
    wait(workers, mutex);
  workers ++;
  unlock(mutex);
}

release_work()
{
  lock(mutex);
  workers--;
  signal(managers);
  signal(worker);
  unlock(mutex);
}

(void *) work (void *)
{
  while(windowsmanagersfinished==1)
  {
    acquire_work();
    ServerBuffer * temp = buffer[counter];
    counter--;
    buffer[counter] = NULL;
    release_work();
    /*
      work
    */
    deleteServerBuffer(temp);
    free(temp);
    temp = NULL;
  }
}


acquire_manager()
{
  lock(mutex);
  while (worker || manager || (counter==BUFFERSIZE))
    wait(managers, mutex);
  manager++;
  unlock(mutex);
}

release_manager()
{
  lock(mutex);
  manager--;
  signal(workers);
  signal(managers);
  unlock(managers);
}

(void *) windowManager(void *)
{
  /*connection, send request*/
  while(/*read is valid*/)
  {

    ServerBuffer * sb;
    acquire_manager();
    sb = createServerBuffer(/*what i read*/, /*ServerName*/, /*Port*/);
    buffer[counter] = sb;
    counter++;
    release_manager();
    sb = NULL;

  }
  acquire_manager();
  windowsmanagersfinished++;
  release_manager();
}
