#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>		/* For bcopy */
#include <unistd.h>
#include <sys/types.h>  /* sockets */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "tools.h"

void read_args(int argc, char * argv[], int * port, char ** MirrorServerAddress, char *** ContentServers, int *count);

int main(int argc, char * argv[])
{
  int port=-1;
  char * MirrorServerAddress = NULL;
  char ** ContentServers;
  int count=0;

  read_args(argc, argv, &port, &MirrorServerAddress, &ContentServers, &count);

  int i;
  printf("Count = %d \n", count);
  for(i=0; i<count; i++)
    printf("Content Server %d: %s \n", i, ContentServers[i]);

  int sock;
  unsigned int serverlen;
  struct sockaddr_in server;
  struct sockaddr *serverptr;
  struct hostent *rem;

  if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    perror_exit("socket");
  }
  if ((rem = gethostbyname(MirrorServerAddress)) == NULL){	/* Find server address */
    perror_exit("gethostbyname");
  }
  server.sin_family = AF_INET;
  bcopy((char *) rem -> h_addr, (char *) &server.sin_addr, rem -> h_length);
  server.sin_port = htons(port);
  serverptr = (struct sockaddr *) &server;
  serverlen = sizeof server;
  if (connect(sock, serverptr, serverlen) < 0){
    perror_exit("connect");
  }

  char buffer[100];
  sprintf(buffer,"%d", count);
  write_data(sock, buffer);
  for(i=0; i<count; i++)
    write_data(sock, ContentServers[i]);

  char * answer;
  if(read_data(sock, &answer)==0)
  {
    printf("Connection Failed\n" );
    exit(EXIT_FAILURE);
  }
  long FilesTransfered = atoi(strtok(answer, " "));
  long long BytesTransfered = atoi(strtok(NULL, " "));
  double average = strtod(strtok(NULL, " "), NULL);
  double dispersion =strtod(strtok(NULL, " "), NULL);
  printf("FilesTransfered %ld, BytesTransfered %lld\n", FilesTransfered, BytesTransfered );
  printf("Average is %.3f Bytes/file\n", average);
  printf("Dispersion is %.3f\n", dispersion);

  /*release the program's sources*/
  free(answer);
  for(i=0; i< count; i++)
  {
    free(ContentServers[i]);
  }
  free(ContentServers);
  free(MirrorServerAddress);
  close(sock);
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
