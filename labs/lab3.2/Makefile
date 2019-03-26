output: base64.o logger.o
	gcc base64.o logger.o -o base64

base64.o: base64.c
	gcc -c base64.c

logger.o: logger.c
	gcc -c logger.c

clean:
	rm *.o base64
