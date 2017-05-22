#include "tools.h"

typedef struct ContentServer{
  char * Address;
  int Port;
  char * dirorfile;
  int delay;
} ContentServer;

void createContentServer(char * addr, int p, char * file, int d );
void deleteContentServer(ContentServer * cs);
