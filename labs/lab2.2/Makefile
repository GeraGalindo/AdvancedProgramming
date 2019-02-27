
output: testLogger.o logger.o
	gcc testLogger.o logger.o -o output

testLogger.o: testLogger.c
	gcc -c testLogger.c

logger.o: logger.c
	gcc -c logger.c

clean:
	rm *.o output
