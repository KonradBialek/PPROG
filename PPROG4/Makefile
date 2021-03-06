CC=gcc
CFLAGS=-Wall

all: a.out

modul.o:modul.c modul.h
	$(CC) -c $(CFLAGS) modul.c

main.o: main.c modul.h
	$(CC) -c $(CFLAGS) main.c

a.out:main.o modul.o
	$(CC) main.o modul.o

