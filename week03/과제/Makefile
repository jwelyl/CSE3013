CC = g++
CFLAGS = -g

TARGET = str
SOURCES = main.cpp Str.cpp
OBJECTS = main.o Str.o

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

.PHONY : clean
clean :
	rm $(TARGET) $(OBJECTS)

main.o : Str.h main.cpp
Str.o : Str.h Str.cpp
