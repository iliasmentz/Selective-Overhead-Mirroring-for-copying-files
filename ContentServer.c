#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>			/* For sockets */
#include <sys/socket.h>			/* For sockets */
#include <netinet/in.h>			/* For Internet sockets */
#include <netdb.h>			/* For gethostbyname */

#include "tools.h"

void read_args(int argc, char * argv[], int * port, char ** dirorfile);
void * ContentChild(void * ptr);

char * dirorfile=NULL;

int main(int argc, char * argv[])
{
  int port=-1;

  read_args(argc, argv, &port, &dirorfile);
  printf("%s \n", dirorfile );

  unsigned int serverlen, clientlen;	/* Server - client variables */
	struct sockaddr_in server, client;
	struct sockaddr *serverptr, *clientptr;
  int sock, newsock;

  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1){	/* Create socket */
    perror_exit("socket");
  }
  server.sin_family = AF_INET;	/* Internet domain */
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);	/* The given port */
	serverptr = (struct sockaddr *) &server;
	serverlen = sizeof server;

  if (bind(sock, serverptr, serverlen) < 0){
    perror_exit("bind");
  }

  if (listen(sock, 100) < 0){
    perror_exit("listen");
  }
  clientptr = (struct sockaddr *) &client;
  clientlen = sizeof(client);
  pthread_t service;
  while((newsock = accept(sock, clientptr, &clientlen)) != -1){
    pthread_create(&service, 0, ContentChild, &newsock);
    //close(newsock);
  }


  free(dirorfile);
  exit(EXIT_SUCCESS);
}

void * ContentChild(void * ptr)
{
  int socket;
  socket = *(int *)ptr;
  char *request;
  read_data(socket, &request);
  printf("%s\n",request);
  char * type = strtok(request, " ");
  if(strcmp(type, "LIST")==0)
  {
    char command [1024];
    sprintf(command, "find %s -type f", dirorfile);
    FILE * fp, *temp;
    fp = popen(command, "r");
    sprintf(command, "%s | wc -l", command);
    temp = popen(command, "r");
    char *line = NULL;
    size_t len = 0;
    int count = 0;
    if(getline(&line, &len, temp)!=0)
      count =atoi(line);
    printf("Count is %d \n", count);
    write_data(socket, line);
    char * pos;
    while( getline(&line, &len, fp) !=-1)
    {
      printf("%s", line);
      if ((pos=strchr(line, '\n')) != NULL)
	    		*pos = '\0';	//we don't want to include the \n in our strings
      write_data(socket, line);
    }
    // printf("Got list request\n");
  }
  else if(strcmp(type, "FETCH")==0)
  {
    write_data(socket, "FETCH");
  }
  free(request);
  close(socket);
  pthread_detach(pthread_self());
  pthread_exit((void *)0);
}

void read_args(int argc, char * argv[], int * port, char ** dirorfile)
{
  int i;

  for (i = 1; i < argc; i+=2)
  {
    if(strcmp(argv[i], "-p") == 0)
    {
      *port =atoi(argv[i+1]);
    }
    else if(strcmp(argv[i], "-d") == 0)
    {
      *dirorfile = malloc((strlen(argv[i+1])+1)*sizeof(char));
      strcpy(*dirorfile, argv[i+1]);
    }
  }

  if((*port < 0) || (*dirorfile == NULL))
  {
    printf("Usage: ./ContentServer -p <port> -d <dirorfilename> \n");
    exit(EXIT_FAILURE);
  }
}
