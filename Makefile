SOURCE = ContentServer.c MirrorServer.c
OBJS = ContentServer.o MirrorServer.o
EXEC = ContentServer MirrorServer
CC	= gcc
FLAGS   = -g -c -Wall -lpthreads

All: $(EXEC)

ContentServer: ContentServer.o
	$(CC) -g $? -o $@

ContentServer.o: ContentServer.c
	$(CC) $(FLAGS) $?

MirrorServer: MirrorServer.o
	$(CC) -g $? -o $@

MirrorServer.o: MirrorServer.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(EXEC)
