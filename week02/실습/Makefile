CC = gcc
CFLAGS = -g

TARGET = fmt
SOURCES = main.cpp Output.cpp String_Manipulation.cpp
OBJECTS = main.o Output.o String_Manipulation.o


$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET)  $(SOURCES)

.PHONY : clean
clean:
	rm $(TARGET) $(OBJECTS)

main.o : Header.h main.cpp
Output.o : Header.h Output.cpp
String_Manipulation.o : Header.h String_Manipulation.cpp
