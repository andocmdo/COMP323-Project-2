CC=gcc
CFLAGS=-pthread -lrt -Wall

all: proj2

proj2: sisinger_andrew_proj2_COMP323.c proj2.h
	$(CC) $(CFLAGS) -o proj2 sisinger_andrew_proj2_COMP323.c

clean:
	rm proj2
