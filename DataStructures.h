#ifndef __DATA_STRUCTS_HEADER__
#define __DATA_STRUCTS_HEADER__

typedef struct ContentServer{
  char * Address;
  int Port;
  char * dirorfile;
  int delay;
}ContentServer;

typedef struct ServerBuffer{
  char * dirorfilename;
  char * ContentServerAddress;
  int port;
}ServerBuffer;

ContentServer * createContentServer(char * addr, int p, char * file, int d );
void deleteContentServer(ContentServer * cs);

ServerBuffer * createServerBuffer(char * dirorfile, char * ServerAddress, int p);
void deleteServerBuffer(ServerBuffer * sb);

#endif
