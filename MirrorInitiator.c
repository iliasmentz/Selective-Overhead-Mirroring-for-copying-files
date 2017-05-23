#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tools.h"

void read_args(int argc, char * argv[], int * port, char ** MirrorServerAddress);

int main(int argc, char * argv[])
{
  int port=-1;
  struct hostent * MirrorServer;
  char * MirrorServerAddress = NULL;
  read_args(argc, argv, &port, &MirrorServerAddress);
  MirrorServer = gethostbyname(MirrorServerAddress);
  printf("%s \n", MirrorServer->h_name);
  free(MirrorServerAddress);
  exit(EXIT_SUCCESS);
}


void read_args(int argc, char * argv[], int * port, char ** MirrorServerAddress)
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
      
    }
  }

  if((*port < 0))
  {
    printf("Usage: ./MirrorServer -p <port> -m <dirname> -w <threadnum>\n");
    exit(EXIT_FAILURE);
  }
}
