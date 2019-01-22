#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#define SING_LINE 0
#define MULT_LINE 1
#define NO_COMMENT 2

int main(int argc, char *argv[])
{
	if( argc == 2 ){
	volatile char ch1, ch2;
	int type = NO_COMMENT;
		FILE *fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL){
		printf("Unable to open the file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	/* Multi-line comment
	*/
	while((ch1 = fgetc(fp)) != EOF){
		// New Line
		if ((ch1 == '\n') && (type != MULT_LINE))
			type = NO_COMMENT;

		// Another comment
		if ((ch1 == '/') && (type != MULT_LINE) && (type != SING_LINE)){
			ch2 = fgetc(fp);
			if (ch2 == '*'){
				type = MULT_LINE;
			}if (ch2 == '/'){
				type = SING_LINE;
			}
		} else {
			type = NO_COMMENT;
			// One more comment
		}
		if ((type != MULT_LINE) && (type != SING_LINE)){
			printf("%c", ch1);
		}
		if (type == MULT_LINE){
			ch1 = ch2;
			ch2 = fgetc(fp);
			while(1){
				if ((ch1 == '*') && (ch2 == '/')){
					ch1 = fgetc(fp);
					type = NO_COMMENT;
					break;
				} else {
					ch1 = ch2;
					ch2 = fgetc(fp);
				}
			}
		}
		if (type == SING_LINE){
			while(1){
				if(ch1 == '\n')
					break;
				ch1 = fgetc(fp);
			}
		}
	}

	fclose(fp);
    } else if (argc > 2)
    	printf("Too many arguments");
    else
    {
	/* Another
	Multi
	line
	comment*/
        printf("ERROR: Not enough arguments\n");
		printf("Usage:\n\n");
		printf("./a.out <input_file_name>\n\n");
    }
    return 0;
}

