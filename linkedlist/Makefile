.PHONY: clean

CFLAGS=-Wall -g 

test: linkedlist.o test.o

clean:
	rm -f test *.o 

re: test clean
	
fe:
	${MAKE} test
	rm -f *.o
	valgrind ./test
	rm test