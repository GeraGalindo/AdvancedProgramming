#include <stdarg.h>
#include <execinfo.h>
#include <stdlib.h>
#include <syslog.h>

#include "logger.h"

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

#define SYSLOG 1
#define STDOUT 0

static int currentLogType = STDOUT;

void textcolor(int attr, int fg, int bg){
	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int initLogger(char* logType){
	if(!strcmp(logType, "syslog")){
		printf("logging into syslog\n");
		currentLogType = SYSLOG;
	} else {
		printf("logging into stdout\n");
		currentLogType = STDOUT;
	}
}

int infof(const char *format, ...){
  va_list argPtr;
  char *p, *sval;
  int ival;
  double dval;

  va_start(argPtr, format);

	if(currentLogType == SYSLOG){
		char msg[50];
		int idx = 0;
		p = format;
		while(*p != '%'){
			msg[idx] = *p;
			p++;
			idx++;
		}
		msg[idx] = '\n';
		syslog(LOG_USER | LOG_INFO, "INFO: %s", msg);
	} else {
		textcolor(BRIGHT, GREEN, BLACK);
	  printf("INFO: ");
	  textcolor(RESET, WHITE, BLACK);
	}

  for(p = format; *p; p++){
    if(*p != '%'){
			if(currentLogType != SYSLOG){
				putchar(*p);
			}
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(argPtr, int);
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_INFO, "%d\n", ival);
				} else {
					printf("%d\n", ival);
				}
        break;
      case 'f':
        dval = va_arg(argPtr, double);
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_INFO, "%f\n", dval);
				} else {
					printf("%f\n", dval);
				}
        break;
      case 's':
        for (sval = va_arg(argPtr, char *); *sval; sval++){
					if(currentLogType == SYSLOG){
						syslog(LOG_USER | LOG_INFO, "%c", *sval);
					} else {
						putchar(*sval);
					}
        }
        break;
      default:
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_INFO, "%c\n", *p);
				} else {
					putchar(*p);
				}
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

	if(currentLogType == SYSLOG){
		char msg[50];
		int idx = 0;
		p = format;
		while(*p != '%'){
			msg[idx] = *p;
			p++;
			idx++;
		}
		msg[idx] = '\n';
		syslog(LOG_USER | LOG_WARNING, "WARN: %s", msg);
	} else {
		textcolor(BRIGHT, YELLOW, BLACK);
	  printf("WARN: ");
	  textcolor(RESET, WHITE, BLACK);
	}

	for(p = format; *p; p++){
    if(*p != '%'){
			if(currentLogType != SYSLOG){
				putchar(*p);
			}
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(argPtr, int);
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_WARNING, "%d\n", ival);
				} else {
					printf("%d\n", ival);
				}
        break;
      case 'f':
        dval = va_arg(argPtr, double);
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_WARNING, "%f\n", dval);
				} else {
					printf("%f\n", dval);
				}
        break;
      case 's':
        for (sval = va_arg(argPtr, char *); *sval; sval++){
					if(currentLogType == SYSLOG){
						syslog(LOG_USER | LOG_WARNING, "%c", *sval);
					} else {
						putchar(*sval);
					}
        }
        break;
      default:
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_WARNING, "%c\n", *p);
				} else {
					putchar(*p);
				}
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

	if(currentLogType == SYSLOG){
		char msg[50];
		int idx = 0;
		p = format;
		while(*p != '%'){
			msg[idx] = *p;
			p++;
			idx++;
		}
		msg[idx] = '\n';
		syslog(LOG_USER | LOG_ERR, "ERROR: %s", msg);
	} else {
		textcolor(BRIGHT, RED, BLACK);
	  printf("ERROR: ");
	  textcolor(RESET, WHITE, BLACK);
	}

	for(p = format; *p; p++){
    if(*p != '%'){
			if(currentLogType != SYSLOG){
				putchar(*p);
			}
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(argPtr, int);
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_ERR, "%d\n", ival);
				} else {
					printf("%d\n", ival);
				}
        break;
      case 'f':
        dval = va_arg(argPtr, double);
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_ERR, "%f\n", dval);
				} else {
					printf("%f\n", dval);
				}
        break;
      case 's':
        for (sval = va_arg(argPtr, char *); *sval; sval++){
					if(currentLogType == SYSLOG){
						syslog(LOG_USER | LOG_ERR, "%c", *sval);
					} else {
						putchar(*sval);
					}
        }
        break;
      default:
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_ERR, "%c\n", *p);
				} else {
					putchar(*p);
				}
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

	if(currentLogType == SYSLOG){
		char msg[50];
		int idx = 0;
		p = format;
		while(*p != '%'){
			msg[idx] = *p;
			p++;
			idx++;
		}
		msg[idx] = '\n';
		syslog(LOG_USER | LOG_EMERG, "PANIC: %s", msg);
	} else {
		textcolor(BRIGHT, BLACK, RED);
	  printf("PANIC:");
	  textcolor(RESET, WHITE, BLACK);
	  printf(" ");
	}

	for(p = format; *p; p++){
    if(*p != '%'){
			if(currentLogType != SYSLOG){
				putchar(*p);
			}
      continue;
    }
    switch (*++p) {
      case 'd':
        ival = va_arg(argPtr, int);
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_EMERG, "%d\n", ival);
				} else {
					printf("%d\n", ival);
				}
        break;
      case 'f':
        dval = va_arg(argPtr, double);
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_EMERG, "%f\n", dval);
				} else {
					printf("%f\n", dval);
				}
        break;
      case 's':
        for (sval = va_arg(argPtr, char *); *sval; sval++){
					if(currentLogType == SYSLOG){
						syslog(LOG_USER | LOG_EMERG, "%c", *sval);
					} else {
						putchar(*sval);
					}
        }
        break;
      default:
				if(currentLogType == SYSLOG){
					syslog(LOG_USER | LOG_EMERG, "%c\n", *p);
				} else {
					putchar(*p);
				}
        break;
    }
  }

  printTrace();
  exit(1);
}
