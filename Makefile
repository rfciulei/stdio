CC = gcc
CFLAGS = -Wall -fPIC -g

all: build

build: libso_stdio.o
	$(CC) -shared libso_stdio.o -o libso_stdio.so
	cp -rf libso_stdio.so ./checker-lin/

libso_stdio.o: libso_stdio.c
	$(CC) $(CFLAGS) -c libso_stdio.c

clean:
	rm -f *.o libso_stdio.so


