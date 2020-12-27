CC = g++
CFLAGS = -g

TARGET = array
SOURCES = main.cpp Array.cpp RangeArray.cpp
OBJECTS = main.o Array.o RangeArray.o

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

.PHONY : clean
clean : 
	rm $(TARGET) $(OBJECTS)

main.o : Array.h RangeArray.h main.cpp
Array.o : Array.h Array.cpp
RangeArray.o : Array.h RangeArray.h RangeArray.cpp
