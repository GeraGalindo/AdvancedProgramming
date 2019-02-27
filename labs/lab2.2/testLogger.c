#include <stdio.h>

int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main(){
  infof("%d\t%s\t%f\n",1 , "information", 1.0);
  warnf("%d\t%s\t%f\n",1 , "warining", 2.0);
  errorf("%d\t%s\t%f\n",1 , "error", 3.0);
  panicf("%d\t%s\t%f\n",1 , "panic", 4.0);
  printf("Statement never reached since panicf() call\n");
}
