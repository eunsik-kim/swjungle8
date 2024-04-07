.PHONY: clean

CFLAGS=-Wall -g -DBST

test: test.o binarysearchtree.o 

clean:
	rm -f test *.o 

fe:
	${MAKE} test
	rm -f *.o
	valgrind ./test
	rm test