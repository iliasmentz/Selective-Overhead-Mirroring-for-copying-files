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

void acquire_work(int haswork)
{
  /*has work means that the thread needs to acquire the cs
  in order to finish the ongoing request from his side*/
  pthread_mutex_lock(&mutex);
  while(worker || manager || (counter == 0) )
  {
    if((windowsmanagersfinished == m) && (counter==0) && (!haswork))
    {
      /*if all the Fetching work is done, go to the allDone cond var*/
      pthread_cond_signal(&workers_cond);
      pthread_mutex_unlock(&mutex);
      pthread_mutex_lock(&mutex2);
      workers_ended++;
      /*if you are not last you sleep*/
      if(workers_ended < w)
      {
        pthread_cond_wait(&allDone, &mutex2);
      }
      pthread_cond_broadcast(&allDone);
      pthread_mutex_unlock(&mutex2);
      pthread_exit(0);
    }
    else if(counter ==0 && !worker && !manager && haswork)
      break;
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
  while(1)
  {
    /*get file to transfer from the critical section*/
    acquire_work(0);
    counter--;
    ServerBuffer * temp = buffer[counter];
    buffer[counter] = NULL;
    //printf("Removed from buffer\n" );
    //printf("Buffer size is: %d\n", counter );
    release_work();

    /*connection as client*/
    int sock;
    unsigned int serverlen;
    struct sockaddr_in server;
    struct sockaddr *serverptr;
    struct hostent *rem;

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
      perror_exit("socket");
    }
    if ((rem = find_hostent(temp->ContentServerAddress)) == NULL){	/* Find server address */
      perror_exit("Couldn't find host");
    }
    server.sin_family = AF_INET;
    bcopy((char *) rem -> h_addr, (char *) &server.sin_addr, rem -> h_length);
    server.sin_port = htons(temp->port);
    serverptr = (struct sockaddr *) &server;
    serverlen = sizeof server;
    if (connect(sock, serverptr, serverlen) < 0){
      perror_exit("connect");
    }

    /*sending FETCH request*/
    char * message;
    char idstring[32];
    sprintf(idstring, "%d", temp->id);
    message = malloc((strlen(temp->dirorfilename)+strlen(idstring)+8)*sizeof(char));
    sprintf(message, "FETCH %s %s", temp->dirorfilename, idstring);
    //printf("Sending %s\n", message );

    /*create the file and the folders that it belongs*/
    char * filename;
    filename = malloc((strlen(temp->ContentServerAddress)+strlen(temp->dirorfilename)+strlen(dirname)+4+strlen(idstring))*sizeof(char));
    sprintf(filename, "%s/%s:%s/%s", dirname, temp->ContentServerAddress, idstring, temp->dirorfilename);
    int filedesc = CreateFile(filename);
    free(filename);
    write_data(sock, message);

    /*write the data to the file*/
    char * answer;
    int  total =0;
    int i;
    while((i=read_data(sock, &answer))>0)
    {
      //printf("i is: %d\n",i );
      total+=i;
      write(filedesc, answer, i);
      free(answer);
      answer = NULL;
    }

    /*write the files stats in order to create the metrics*/
    acquire_work(1);
    if(dispersionsize == dispersioncounter)
    {
      dispersionsize*=2;
      dispersion = realloc(dispersion, dispersionsize*sizeof(int));
    }
    BytesTransfered+=total;
    FilesTransfered++;
    dispersion[dispersioncounter] = BytesTransfered;
    dispersioncounter++;
    release_work();

    /*release the program's sources*/
    close(filedesc);
    close(sock);
    free(message);
    deleteServerBuffer(temp);
    free(temp);
    temp = NULL;
  }
  pthread_exit((void *)0); /*never gonna get there, but just in case :D */
}
