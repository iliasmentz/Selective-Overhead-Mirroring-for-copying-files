#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_args(int argc, char * argv[], int * port, char ** dirname, int * threadnum);

int main(int argc, char * argv[])
{
  int port=-1;
  char * dirname=NULL;
  int threadnum =1;
  read_args(argc, argv, &port, &dirname, &threadnum);
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
