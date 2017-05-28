SOURCE = ContentServer.c MirrorServer.c MirrorInitiator.c MirrorManager.c Worker.c tools.c test.c DataStructures.c
OBJS = ContentServer.o MirrorServer.o MirrorInitiator.o MirrorManager.o Worker.o tools.o test.o DataStructures.o
EXEC = ContentServer MirrorInitiator MirrorServer
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

MirrorInitiator: MirrorInitiator.o tools.o
	$(CC) -g $? -o $@

MirrorServer: MirrorServer.o MirrorManager.o Worker.o DataStructures.o tools.o
	$(CC) -g $? -o $@ -lpthread

MirrorServer.o: MirrorServer.c
	$(CC) $(FLAGS) $?


MirrorInitiator.o: MirrorInitiator.c
	$(CC) $(FLAGS) $?

MirrorManager.o: MirrorManager.c
	$(CC) $(FLAGS) $?

Worker.o: Worker.c
	$(CC) $(FLAGS) $?

DataStructures.o: DataStructures.c
	$(CC) $(FLAGS) $?

tools.o: tools.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(EXEC) test
