# COMP1927 14s2 - Assignment 1 Makefile

CC=gcc
CFLAGS=-Wall -Werror -gdwarf-2

all: vlad vladtest

vlad : vlad.o allocator.o

vladtest : vladtest.o

vlad.o : vlad.c allocator.h

vladtest.o : vladtest.c

allocator.o : allocator.c allocator.h

clean :
	rm -f vlad *.o
