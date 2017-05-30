#ifndef __CONTENT_SERVER_HEADER__
#define __CONTENT_SERVER_HEADER__

typedef struct delayID{
  int ID;
  int delay;
}delayID;

extern pthread_mutex_t mutex;
extern int counter;
extern delayID * delays;
extern int buffersize;
extern int reader;
extern int writer;
extern pthread_cond_t readers_cond;
extern pthread_cond_t writers_cond;

void * ContentChild(void * ptr);
void reader_lock();
void reader_release();
void writer_lock();
void writer_release();

#endif
