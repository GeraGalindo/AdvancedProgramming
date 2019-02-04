/*
	HOW TO LINK
	gcc -c str-main.c -o str-main.o
	gcc -c strlib.c -o strlib.o
	gcc str-main.o strlib.o -o str-final.o
*/
#include <stdio.h>

int strlen(char *str);
char *stradd(char *origin, char *addition);
int strfind(char *origin, char *substr);

int main(int argc, char *argv[]){

  if (argc != 4) {
		printf("Usage: \na.out originalVeryLongString Addition Add\n");
		return 0;
	} else {
		char *origStr = argv[1]; // Original String
    char *catStr = argv[2]; // String to be concatenated
    char *subStr = argv[3];

    printf("Initial Lenght\t\t: %d\n", strlen(origStr));
    printf("New String\t\t: %s\n", stradd(origStr, catStr));
    printf("SubString was found\t: %s\n", strfind(catStr, subStr) ? "Yes": "No");
	}

  return 0;
}
