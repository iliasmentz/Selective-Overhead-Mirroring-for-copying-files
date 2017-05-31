SOURCE = ContentServer.c MirrorServer.c MirrorInitiator.c MirrorManager.c Worker.c ReaderWriter.c tools.c DataStructures.c
OBJS = ContentServer.o MirrorServer.o MirrorInitiator.o MirrorManager.o Worker.o ReaderWriter.o tools.o DataStructures.o
EXEC = ContentServer MirrorInitiator MirrorServer
CC	= gcc
FLAGS   = -g -c

All: $(EXEC)

ContentServer: ContentServer.o ReaderWriter.o tools.o
	$(CC) -g $? -o $@ -lpthread

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

ReaderWriter.o: ReaderWriter.c
	$(CC) $(FLAGS) $?

DataStructures.o: DataStructures.c
	$(CC) $(FLAGS) $?

tools.o: tools.c
	$(CC) $(FLAGS) $?

clean:
	rm -f $(OBJS) $(EXEC)
