#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataStructures.h"
#include "tools.h"


ContentServer * createContentServer(char * addr, int p, char * file, int d )
{
  ContentServer * cs;
  cs = malloc(sizeof(ContentServer));

  cs->Address = copystring(addr);
  cs->Port = p;
  cs->dirorfile = copystring(file);
  cs->delay = d;
  return cs;
}

void deleteContentServer(ContentServer * cs)
{
 free(cs->Address);
 free(cs->dirorfile);
}

ServerBuffer * createServerBuffer(char * dirorfile, char * ServerAddress, int p)
{
  ServerBuffer * sb;
  sb = malloc(sizeof(ServerBuffer));
  sb->dirorfilename = copystring(dirorfile);
  sb->ContentServerAddress = copystring(ServerAddress);
  sb->port = p;
  return sb;
}

void deleteServerBuffer(ServerBuffer * sb)
{
  free(sb->dirorfilename);
  free(sb->ContentServerAddress);
}
