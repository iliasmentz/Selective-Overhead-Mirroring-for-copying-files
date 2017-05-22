#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_args(int argc, char * argv[], int * port, char ** dirorfile);

int main(int argc, char * argv[])
{
  int port=-1;
  char * dirorfile=NULL;
  read_args(argc, argv, &port, &dirorfile);
  printf("%s \n", dirorfile );
  free(dirorfile);
  exit(EXIT_SUCCESS);
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
