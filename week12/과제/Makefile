CC = gcc
CFLAGS = -g

TARGET = maze 
SOURCES = mazeGen.c 
OBJECTS = mazeGen.o 

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

.PHONY : clean
clean :
	rm $(TARGET) $(OBJECTS)

mazeGen.o : mazeGen.c
