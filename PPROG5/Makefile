CC=gcc
CFLAGS=-Wall

all: a.out

stos.o:stos.c stos.h
	$(CC) -c $(CFLAGS) stos.c

kalkulator.o: kalkulator.c stos.h
	$(CC) -c $(CFLAGS) kalkulator.c

a.out:kalkulator.o stos.o
	$(CC) kalkulator.o stos.o






