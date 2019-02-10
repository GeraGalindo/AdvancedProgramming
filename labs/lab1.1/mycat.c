#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]){
  clock_t start_time = clock();
  int inputFd, outputFd, openFlags;
  mode_t filePerms;
  ssize_t numRead;
  char buf[BUF_SIZE];

  if(argc != 2)
    printf("Usage: ./output <file to copy>\n");

  inputFd = open(argv[1], O_RDONLY);
  if (inputFd == -1)
    printf("Error while opening the file %s\n", argv[1]);

  openFlags = O_CREAT | O_WRONLY | O_TRUNC;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  outputFd = open("/dev/tty", openFlags, filePerms);
  if (outputFd == -1)
    printf("Error while oppening /dev/tty\n");

  // Transfer data until the end of the file or an error occurs
  while((numRead = read(inputFd, buf, BUF_SIZE)) > 0){
    if (write(outputFd, buf, numRead) != numRead){
      printf("Couldn't write whole buffer\n");
    }
  }
  if (numRead == -1)
      printf("Error while reading\n");

  if (close(inputFd) == -1)
      printf("Error while closing the input file");
  if (close(outputFd) == -1)
      printf("Error while closing /dev/tty");

  clock_t end_time = clock();
  printf("Elapsed: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

  return 0;
}
