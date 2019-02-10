#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
  printf("Lab 1.2 - PTY/TTY Broadcaster\n");
  int iFd, oFd, oFlags;
  mode_t filePerms;
  char buf[2];
  char fPath[256];
  char message[256];

  if(argc < 2){
    printf("Usage: ./output message (it can include white-spaces)\n");
  }

  for(int i = 1; i < argc; ++i){
    strcat(message, argv[i]);
    strcat(message, " ");
  }
  printf("Full message: %s\n", message);
  printf("\n");

  int terminalCount;
  system("ls /dev/pts/  | wc -l > count.txt");
  iFd = open("count.txt", O_RDONLY);
  if(iFd == -1){
    printf("Error while opening the file %s\n", argv[1]);
  }

  read(iFd, buf, 2); // it could only read from 0 to 99

  terminalCount = atoi(buf) - 1;

  printf("Terminals Open = %d\n", terminalCount);

  oFlags = O_CREAT | O_WRONLY | O_TRUNC;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;


  // Now we need to loop and send message to each open terminal
  for (int i = 0; i < terminalCount; ++i){
    sprintf(fPath, "/dev/pts/%d", i);
    oFd = open(fPath, oFlags, filePerms);
    if (write(oFd, message, strlen(message)) != strlen(message)){
      printf("Couldn't write whole buffer\n");
    }
  }

  if (close(iFd) == -1)
      printf("Error while closing the input file");
  if (close(oFd) == -1)
      printf("Error while closing /dev/tty");
  return 0;
}
