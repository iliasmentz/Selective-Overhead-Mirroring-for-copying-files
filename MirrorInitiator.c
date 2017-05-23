#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void read_args(int argc, char * argv[], int * port, struct hostent ** MirrorServer);

int main(int argc, char * argv[])
{
  int port=-1;
  struct hostent *MirrorServer = malloc(sizeof(struct hostent));
  read_args(argc, argv, &port, &MirrorServer );
  printf("%s \n", MirrorServer->h_name);
  free(MirrorServer);
  exit(EXIT_SUCCESS);
}


void read_args(int argc, char * argv[], int * port, struct hostent ** MirrorServer)
{
  int i;
  struct in_addr myaddress;
  for (i = 1; i < argc;)
  {
    if(strcmp(argv[i], "-p") == 0)
    {
      *port =atoi(argv[i+1]);
      i+=2;
    }
    else if(strcmp(argv[i], "-n") == 0)
    {
      if((*MirrorServer = gethostbyname(argv[i+1])) != NULL)
      {
        i+=2;
      }
      else{
        printf("Its IP\n" );
        inet_aton(argv[i+1], &myaddress);
        if ((*MirrorServer = gethostbyaddr((const char*)&myaddress, sizeof(myaddress), AF_INET)) == NULL)
        {
          printf("Could not resolved Name/Address:  %s\n",argv[i+1]);
          exit(EXIT_FAILURE);
        }
        i+=2;
      }
    }
  }

  if((*port < 0))
  {
    printf("Usage: ./MirrorServer -p <port> -m <dirname> -w <threadnum>\n");
    exit(EXIT_FAILURE);
  }
}
