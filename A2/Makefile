OBJS = LineEditor.o Command.o main.o
CC = g++
DEBUG = 
CFLAGS = -Wall -std=c++11 -c $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)

all : led

led : $(OBJS) 
	$(CC) $(LFLAGS) $(OBJS) -o led

main.o : LineEditor.h main.cpp 
	$(CC) $(CFLAGS) main.cpp

LineEditor.o : LineEditor.h Command.h LineEditor.cpp
	$(CC) $(CFLAGS) LineEditor.cpp

LinkedList.o : Command.h Command.cpp
	$(CC) $(CFLAGS) Command.cpp

clean :
	rm *.o led
