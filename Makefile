PROG = bin/board 
CC = g++

CPPFLAGS = -g -Wall -std=c++11
OBJS = board.o main.o

$(PROG) : $(OBJS)
	$(CC) $(CPPFLAGS) -o $(PROG) $(OBJS)
board.o : 
	$(CC) $(CPPFLAGS) -c src/board.cpp
main.o : 
	$(CC) $(CPPFLAGS) -c src/main.cpp

clean: 
	rm -f core $(PROG) $(OBJS)
