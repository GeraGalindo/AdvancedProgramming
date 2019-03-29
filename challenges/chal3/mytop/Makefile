output: mytop.o logger.o
	gcc mytop.o logger.o -o mytop

mytop.o: mytop.c
	gcc -c mytop.c

logger.o: logger.c
	gcc -c logger.c

clean:
	rm *.o mytop
