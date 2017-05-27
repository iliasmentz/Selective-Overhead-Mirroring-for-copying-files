#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>                         /* sockets */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "tools.h"

void read_args(int argc, char * argv[], int * port, char ** MirrorServerAddress, char *** ContentServers, int *count);

int main(int argc, char * argv[])
{
  int port=-1;
  struct hostent * MirrorServer;
  char * MirrorServerAddress = NULL;
  char ** ContentServers;
  int count=0;

  MirrorServer = gethostbyname(MirrorServerAddress);

  int i;
  printf("Count = %d \n", count);
  for(i=0; i<count; i++)
    printf("Content Server %d: %s \n", i, ContentServers[i]);

  struct sockaddr_in myaddr;  /* build our address here */
  int	c, lsock, csock;  /* listening and client sockets */
  FILE	*sock_fp;             /* stream for socket IO */
  /* Step 1: Get a socket */
  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1 )
      perror_exit( "socket" );
  /* Step 2: lookup server's address and connect there */
  if ((hp = gethostbyname(MirrorServerAddress)) == NULL) {
    herror("gethostbyname");
    exit(1);
  }
  printf("Hostname: %s \n", MirrorServer->h_name);
  memcpy(&servadd.sin_addr, hp->h_addr, hp->h_length);
  servadd.sin_port = htons(PORTNUM); /* set port number */
  servadd.sin_family = AF_INET ;     /* set socket type */
  if (connect(sock, (struct sockaddr*) &servadd, sizeof(servadd)) !=0)
      perror_exit( "connect" );

  write(sock, )
  for(i=0; i< count; i++)
  {
    free(ContentServers[i]);
  }
  free(ContentServers);
  free(MirrorServerAddress);
  exit(EXIT_SUCCESS);
}


void read_args(int argc, char * argv[], int * port, char ** MirrorServerAddress, char *** ContentServers, int *count)
{
  int i;
  for (i = 1; i < argc;)
  {
    if(strcmp(argv[i], "-p") == 0)
    {
      *port =atoi(argv[i+1]);
      i+=2;
    }
    else if(strcmp(argv[i], "-n") == 0)
    {
      *MirrorServerAddress = copystring(argv[i+1]);
      i+=2;
    }
    else if(strcmp(argv[i], "-s")==0)
    {
      int j;
      for(j=i+1; j<argc; j++)
      {
        if ((strcmp(argv[j], "-n")==0) || (strcmp(argv[j], "-p")==0))
          break;
        (*count)++;
      }
      if(*count ==0)
      {
        printf("Usage: ./MirrorInitiator -n <MirrorServerAddress> -p <MirrorServerPort> -s <ContentServers>\n");
        exit(EXIT_FAILURE);
      }
      *ContentServers = malloc((*count) *sizeof(char *));
      for(j=0; j<(*count); j++)
      {
        (*ContentServers)[j] = copystring(argv[i+1+j]);
      }
      i += (*count)+1;
    }
  }

  if((*port < 0) || (*MirrorServerAddress == NULL) || (*count==0))
  {
    printf("Usage: ./MirrorInitiator -n <MirrorServerAddress> -p <MirrorServerPort> -s <ContentServers>\n");
    exit(EXIT_FAILURE);
  }
}
