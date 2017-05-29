#ifndef __DATA_STRUCTS_HEADER__
#define __DATA_STRUCTS_HEADER__

typedef struct ContentServer{
  char * Address;
  int Port;
  char * dirorfile;
  int delay;
  int id;
}ContentServer;

typedef struct ServerBuffer{
  char * dirorfilename;
  char * ContentServerAddress;
  int port;
  int id;
}ServerBuffer;

ContentServer * createContentServer(char * addr, int p, char * file, int d, int ID);
void deleteContentServer(ContentServer * cs);

ServerBuffer * createServerBuffer(char * dirorfile, char * ServerAddress, int p, int ID);
void deleteServerBuffer(ServerBuffer * sb);

#endif
