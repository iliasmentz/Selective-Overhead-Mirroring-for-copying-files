SOURCE = ContentServer.c MirrorServer.c MirrorInitiator.c
OBJS = ContentServer.o MirrorServer.o MirrorInitiator.o
EXEC = ContentServer MirrorServer MirrorInitiator
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

MirrorInitiator: MirrorInitiator.o
	$(CC) -g $? -o $@

MirrorInitiator.o: MirrorInitiator.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(EXEC)
