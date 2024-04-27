CC = gcc
CFLAGS = -O0 -g -Wall -I ..

all: adder

adder: adder.c
	$(CC) $(CFLAGS) -o adder adder.c ../csapp.c

clean:
	rm -f adder *~
