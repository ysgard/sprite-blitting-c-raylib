CC = c99
CFLAGS = -Wall -O3 -g
LDLIBS = -lm -lraylib

all: sprite-blitting

sprite-blitting: sprite-blitting.o
		$(CC) $(LDFLAGS) -o sprite-blitting $(LDLIBS) sprite-blitting.o

sprite-blitting.o: sprite-blitting.c
		$(CC) -c $(CFLAGS) sprite-blitting.c

clean:
		rm -f sprite-blitting sprite-blitting.o
