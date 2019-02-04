
output: str-main.o strlib.o
	gcc str-main.o strlib.o -o output

str-main.o: str-main.c
	gcc -c str-main.c

strlib.o: strlib.c
	gcc -c strlib.c

clean:
	rm *.o output
