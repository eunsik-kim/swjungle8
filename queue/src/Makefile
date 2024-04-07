.PHONY: clean

CFLAGS=-Wall -g
linkeddir=../linkedlist/

queue: queue.o ${linkeddir}linkedlist.o  

clean:
	rm -f queue *.o ${linkeddir}*.o

fe: 
	${MAKE} queue 
	valgrind ./queue
	clean
