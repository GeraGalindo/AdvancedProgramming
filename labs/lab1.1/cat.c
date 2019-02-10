#include <stdio.h>
#include <time.h>

/* filecopy:  copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)
{
  int c;

  while ((c = getc(ifp)) != EOF)
    putc(c, ofp);

}

int main(int argc, char *argv[]){
  clock_t start_time = clock();
  FILE *fp;
  void filecopy(FILE *, FILE *);
  char *prog = argv[0];   /* program name for errors */

  if (argc == 1)  /* no args; copy standard input */
      filecopy(stdin, stdout);
  else
    while (--argc > 0) {
      if ((fp = fopen(*++argv, "r")) == NULL) {
        fprintf(stderr, "%s: can′t open %s\n", prog, *argv);
        return 1;
      } else {
        filecopy(fp, stdout);
        fclose(fp);
      }
    }

  if (ferror(stdout)) {
    fprintf(stderr, "%s: error writing stdout\n", prog);
    return 2;
  }

  clock_t end_time = clock();
  printf("Elapsed: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

  return 0;
}
