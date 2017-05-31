#ifndef __MIRROR_MANAGER_WORKER_HEADER__
#define __MIRROR_MANAGER_WORKER_HEADER__

#include "DataStructures.h"

#define BUFFERSIZE 1024

extern ServerBuffer * buffer[1024];
extern pthread_mutex_t mutex;
extern int counter; 
extern int worker ;
extern int manager;
extern int w ;
extern int m ;
extern pthread_cond_t managers_cond;
extern pthread_cond_t workers_cond;
extern int windowsmanagersfinished;
extern ContentServer ** contentserver;
extern char * dirname;

void acquire_work();
void release_work();
void * work (void *ptr);

void acquire_manager();
void release_manager();
void * mirrorManager(void *ptr);


#endif
