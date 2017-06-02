#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>			/* For sockets */
#include <sys/socket.h>			/* For sockets */
#include <netinet/in.h>			/* For Internet sockets */
#include <netdb.h>			/* For gethostbyname */

#include "MirrorServer.h"
#include "DataStructures.h"
#include "tools.h"

void * mirrorManager(void *ptr)
{
  ContentServer * cs = (ContentServer *)ptr;

  /*connection as client*/
  int sock;
  unsigned int serverlen;
  struct sockaddr_in server;
  struct sockaddr *serverptr;
  struct hostent *rem;

  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    perror_exit("socket");
  }
  if ((rem = gethostbyname(cs->Address)) == NULL){	/* Find server address */
    perror_exit("gethostbyname");
  }
  server.sin_family = AF_INET;
  bcopy((char *) rem -> h_addr, (char *) &server.sin_addr, rem -> h_length);
  server.sin_port = htons(cs->Port);
  serverptr = (struct sockaddr *) &server;
  serverlen = sizeof server;
  if (connect(sock, serverptr, serverlen) < 0){
    perror_exit("connect");
  }

  /*send the LIST request*/
  char localbuffer[127];
  sprintf(localbuffer, "LIST %d %d", cs->id,cs->delay);
  write_data(sock, localbuffer);
  /*how many lines the response is gonna be*/
  char * answer;
  read_data(sock, &answer);
  int count = atoi(answer);
  free(answer);

  /*read all the files that the Content Servers LIST and count those we need*/
  char ** list;
  list = malloc(count *sizeof(char*));
  int i;
  int serverbuffers =0;
  for(i=0; i<count; i++)
  {
    read_data(sock, &list[i]);
    if(strstr(list[i], cs->dirorfile)!=NULL)
      serverbuffers++;
  }
  printf("I will take %d items from %s %s\n", serverbuffers, cs->Address, cs->dirorfile);

  /* find the files that we need in the LIST response*/
  int j=0;
  ServerBuffer ** sb;
  sb = malloc(serverbuffers*sizeof(ServerBuffer*));
  for(i=0; i<count; i++)
  {
    if(strstr(list[i], cs->dirorfile)!=NULL)
    {
      //printf("%s %s %d\n", list[i], cs->Address, cs->Port );
      sb[j] = createServerBuffer(list[i], cs->Address, cs->Port, cs->id);
      j++;
    }

  }
  for(i =0; i<count; i++)
    free(list[i]);
  free(list);
  /*connection, send request*/
  for(i=0 ; i<serverbuffers ; i++)
  {
    /*write one-by-one of the list results in the buffer*/
    acquire_manager();
    buffer[counter] = sb[i];
    counter++;
    release_manager();
  }

  acquire_manager();
  windowsmanagersfinished++;
  release_manager();
  /*release the program's sources*/
  deleteContentServer(cs);
  free(sb);
  pthread_exit((void *)0);
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
