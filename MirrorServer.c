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


void read_args(int argc, char * argv[], int * port, char ** dirname, int * threadnum);

ServerBuffer * buffer[BUFFERSIZE];
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
int dispersionsize=128;
int dispersioncounter=0;
int *dispersion=NULL;
int counter=0;
int worker =0;
int manager =0;
int w =0;
int m =0;
int workers_ended =0;
pthread_cond_t allDone;
pthread_cond_t managers_cond;
pthread_cond_t workers_cond;
int windowsmanagersfinished=0;
ContentServer ** contentserver;
char * dirname=NULL;
long long BytesTransfered = 0;
long FilesTransfered =0;

int main(int argc, char * argv[])
{
  int port=-1;
  int threadnum =-1;

  read_args(argc, argv, &port, &dirname, &threadnum);
  dispersion = malloc(dispersionsize*sizeof(int));
  printf("%s \n", dirname );
  CreateFolder(dirname);
  w=threadnum;
  pthread_t worker[w];
  pthread_mutex_init(&mutex, 0);
  pthread_mutex_init(&mutex2, 0);
  pthread_cond_init(&managers_cond, 0);
  pthread_cond_init(&workers_cond, 0);
  pthread_cond_init(&allDone, 0);

  unsigned int serverlen, clientlen;	/* Server - client variables */
	struct sockaddr_in server, client;
	struct sockaddr *serverptr, *clientptr;
	struct hostent *rem;
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

  if (listen(sock, 1) < 0){
    perror_exit("listen");
  }
  clientptr = (struct sockaddr *) &client;
  clientlen = sizeof(client);
  if ((newsock = accept(sock, clientptr, &clientlen)) < 0){
    perror_exit("accept");
  }
  if ((rem = gethostbyaddr((char *) &client.sin_addr.s_addr, sizeof client.sin_addr.s_addr, client.sin_family)) == NULL) {
    perror("gethostbyaddr");exit(-1);
  }

  printf("Initiator connected %s\n",rem->h_name);

  char *buffer;
  read_data(newsock, &buffer);
  int count= atoi(buffer);
  m = count;
  pthread_t manager[m];
  char ** ContentServers = malloc(count*sizeof(char *));
  contentserver = malloc(count*sizeof(ContentServer *));
  int i;
  char * subfolder;
  for(i =0 ; i < m; i++)
  {
    read_data(newsock, &ContentServers[i]);
    char * addr = strtok(ContentServers[i], ":");
    int port = atoi(strtok(NULL, ":"));
    char * dirorfile = strtok(NULL, ":");
    int delay = atoi(strtok(NULL, ":"));
    //subfolder = malloc((strlen(dirorfile)+strlen(dirname)+2)*sizeof(char));
    //sprintf(subfolder, "%s/%s", dirname, dirorfile);
    //CreateFolder(subfolder);

    contentserver[i] = createContentServer(addr, port, dirorfile, delay, i);
    pthread_create(&manager[i], 0  , mirrorManager, contentserver[i]);
    printf("Server %d is : %s\n", i, ContentServers[i]);
  }
  for(i=0; i < w; i++)
    pthread_create(&worker[i], 0  , work, 0);

  for(i=0; i < m; i++)
    pthread_join(manager[i], 0);
  printf("Managers finished!\n" );
  for(i=0; i < w; i++)
    pthread_join(worker[i], 0);
  printf("Workers finished!\n" );

  double average = (double) BytesTransfered/ (double)FilesTransfered;
  double var=0.0;
  for(i=0; i<dispersioncounter; i++)
    var += (double) ((dispersion[i]-average)*(dispersion[i]-average))/average;
  char answer[3072];
  memset(answer, '\0', 3072);
  sprintf(answer, "%ld %lld %0.3f %0.3f", FilesTransfered, BytesTransfered, average, var);
//  sprintf(answer, "%ld %lld", FilesTransfered, BytesTransfered);
  write_data(newsock, answer);

  printf("Transfered %ld files and %lld bytes. Average: %0.3f, Dispersion %0.3f\n", FilesTransfered, BytesTransfered, average, var );
  free(dispersion);
  pthread_cond_destroy(&allDone);
  pthread_cond_destroy(&managers_cond);
  pthread_cond_destroy(&workers_cond);
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex2);
  for(i = 0 ; i <count; i++)
  {
    free(contentserver[i]);
    free(ContentServers[i]);
  }
  free(contentserver);
  free(ContentServers);
  free(buffer);
  free(dirname);
  close(newsock);
  close(sock);
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
