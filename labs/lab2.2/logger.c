#include <stdarg.h>
#include <execinfo.h>
#include <stdlib.h>

#define RESET		0
#define BRIGHT 		1
#define DIM		2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

void textcolor(int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infof(const char *format, ...){
  va_list argPtr;
  char *p, *sval;
  int ival;
  double dval;

  va_start(argPtr, format);

  textcolor(BRIGHT, GREEN, BLACK);
  printf("INFO: ");
  textcolor(RESET, WHITE, BLACK);


  for(p = format; *p; p++){
    if(*p != '%'){
      putchar(*p);
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(argPtr, int);
        printf("%d", ival);
        break;
      case 'f':
        dval = va_arg(argPtr, double);
        printf("%f", dval);
        break;
      case 's':
        for (sval = va_arg(argPtr, char *); *sval; sval++){
          putchar(*sval);
        }
        break;
      default:
        putchar(*p);
        break;
    }
  }
}

int warnf(const char *format, ...){
  va_list argPtr;
  char *p, *sval;
  int ival;
  double dval;

  va_start(argPtr, format);

  textcolor(BRIGHT, YELLOW, BLACK);
  printf("WARN: ");
  textcolor(RESET, WHITE, BLACK);

  for(p = format; *p; p++){
    if(*p != '%'){
      putchar(*p);
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(argPtr, int);
        printf("%d", ival);
        break;
      case 'f':
        dval = va_arg(argPtr, double);
        printf("%f", dval);
        break;
      case 's':
        for (sval = va_arg(argPtr, char *); *sval; sval++){
          putchar(*sval);
        }
        break;
      default:
        putchar(*p);
        break;
    }
  }
}

int errorf(const char *format, ...){
  va_list argPtr;
  char *p, *sval;
  int ival;
  double dval;

  va_start(argPtr, format);

  textcolor(BRIGHT, RED, BLACK);
  printf("ERROR: ");
  textcolor(RESET, WHITE, BLACK);

  for(p = format; *p; p++){
    if(*p != '%'){
      putchar(*p);
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(argPtr, int);
        printf("%d", ival);
        break;
      case 'f':
        dval = va_arg(argPtr, double);
        printf("%f", dval);
        break;
      case 's':
        for (sval = va_arg(argPtr, char *); *sval; sval++){
          putchar(*sval);
        }
        break;
      default:
        putchar(*p);
        break;
    }
  }
}

void printTrace(){
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  printf ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
     printf ("%s\n", strings[i]);

  free (strings);
}

int panicf(const char *format, ...){
  va_list argPtr;
  char *p, *sval;
  int ival;
  double dval;

  va_start(argPtr, format);

  textcolor(BRIGHT, BLACK, RED);
  printf("PANIC:");
  textcolor(RESET, WHITE, BLACK);
  printf(" ");

  for(p = format; *p; p++){
    if(*p != '%'){
      putchar(*p);
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(argPtr, int);
        printf("%d", ival);
        break;
      case 'f':
        dval = va_arg(argPtr, double);
        printf("%f", dval);
        break;
      case 's':
        for (sval = va_arg(argPtr, char *); *sval; sval++){
          putchar(*sval);
        }
        break;
      default:
        putchar(*p);
        break;
    }
  }

  printTrace();
  exit(1);
}
