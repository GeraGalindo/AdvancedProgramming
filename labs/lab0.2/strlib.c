// strlib.c
#include <stdlib.h>
int strlen(char *str){
  int idx = 0;
  while(str[idx]){
    ++idx;
  }
  return idx;
}

char *stradd(char *origin, char *addition){
  char *retVal = 0;
  int origLen = strlen(origin);
  int addLen = strlen(addition);
  retVal = (char *)malloc(origLen+addLen+1);
  for (int i = 0; i < origLen; ++i){
    retVal[i] = origin[i];
  }
  for (int i = 0; i < (addLen+1); ++i){
    retVal[i+origLen] = addition[i];
  }
  return retVal;
}

int strfind(char *origin, char *substr){
  int origLen = strlen(origin);
  int subsLen = strlen(substr);
  int subStrIdx = 0;
  for(int or = 0; or < origLen; ++or){
    if(subStrIdx == subsLen){
		  return 1;
	  }
	  if(origin[or] == substr[subStrIdx]){
		  ++subStrIdx;
	  } else {
		  subStrIdx = 0;
	  }
  }
  return 0;
}
