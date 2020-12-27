CC = gcc
CFLAGS = -g

TARGET = maze 
SOURCES = mazeGen.c main.c
OBJECTS = mazeGen.o main.o

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

.PHONY : clean
clean :
	rm $(TARGET) $(OBJECTS)

mazeGen.o : mazeGen.h mazeGen.c
main.o : mazeGen.h main.c
