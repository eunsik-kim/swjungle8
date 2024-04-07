.PHONY: test

CFLAGS= -I ../src -Wall -g
linkeddir = ../../linkedlist

cleantest: test clean

test: test.o ../src/queue.o ${linkeddir}/linkedlist.o
	gcc test.o ../src/queue.o ${linkeddir}/linkedlist.o -o test
	valgrind ./test

../src/queue.o:
	$(MAKE) -C ../src queue.o 

${linkeddir}/linkedlist.o:
	$(MAKE) -C ${linkeddir} linkedlist.o 

clean:
	rm -f test *.o ../src/*.o ${linkeddir}/linkedlist.o
	
