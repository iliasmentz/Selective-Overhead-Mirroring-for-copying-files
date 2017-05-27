SOURCE = ContentServer.c MirrorServer.c MirrorInitiator.c tools.c test.c DataStructures.c
OBJS = ContentServer.o MirrorServer.o MirrorInitiator.o tools.o test.o DataStructures.o
EXEC = ContentServer MirrorInitiator
CC	= gcc
FLAGS   = -g -c -Wall

All: $(EXEC)

test: test.o tools.o DataStructures.o
	$(CC) -g $? -o $@ -lpthread

ContentServer: ContentServer.o
	$(CC) -g $? -o $@

test.o: test.c
	$(CC) $(FLAGS) $?

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

DataStructures.o: DataStructures.c
	$(CC) $(FLAGS) $?

tools.o: tools.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(EXEC) test
