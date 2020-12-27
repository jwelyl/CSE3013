CC = g++
CFLAGS = -g

TARGET = garr
SOURCES = main.cpp
OBJECTS = main.o

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

.PHONY : clean
clean :
	rm $(TARGET) $(OBJECTS)

main.o : main.cpp Array.h GrowableArray.h

