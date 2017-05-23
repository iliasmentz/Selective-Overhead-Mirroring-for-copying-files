SOURCE = ContentServer.c MirrorServer.c MirrorInitiator.c tools.c
OBJS = ContentServer.o MirrorServer.o MirrorInitiator.o tools.o
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

MirrorInitiator: MirrorInitiator.o tools.o
	$(CC) -g $? -o $@

MirrorInitiator.o: MirrorInitiator.c
	$(CC) $(FLAGS) $?

tools.o: tools.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(EXEC)
