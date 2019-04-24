PROG = bin/board 
CC = g++

CPPFLAGS = -g -Wall -std=c++11
OBJS = board.o main.o
SRCS = src/board.cpp src/main.cpp

all: $(PROG)

$(PROG) : $(OBJS) Makefile
	$(CC) $(CPPFLAGS) -o $(PROG) $(OBJS)

$(OBJS) : 
	$(CC) $(CPPFLAGS) -c $(SRCS)

clean: 
	rm -f core $(PROG) $(OBJS)
