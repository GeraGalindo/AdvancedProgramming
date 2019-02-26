#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100

struct tnode {
  char *word;
  int count;
  struct tnode *left;
  struct tnode *right;
};

int getword(char *word, int lim){
  int c, getch(void);
  void ungetch(int);
  char *w = word;

  while(isspace(c = getch()));

  if (c !=  EOF){
    *w++ = c;
  }
  if(!isalpha(c)){
    *w = '\0';
    return c;
  }
  for ( ; --lim > 0; w++){
    if (!isalnum(*w = getch())) {
      ungetch(*w);
      break;
    }
  }

  *w = '\0';
  return word[0];
}

char *str_dup(char *s){
  char *p;

  p = (char *) malloc (strlen(s)+1);
  if (p != NULL){
    strcpy(p, s);
  }
  return p;
}

struct tnode *talloc (void){
  return (struct tnode *) malloc(sizeof(struct tnode));
}

void treePrint(struct tnode *p){
  if (p != NULL){
    treePrint(p->left);
    printf("%4d %s\n", p->count, p->word);
    treePrint(p->right);
  }
}

struct tnode *addtree(struct tnode *p, char *w){
  int cond;

  if (p == NULL) {
    p = talloc();
    p->word = str_dup(w);
    p->count = 1;
    p->left = p->right = NULL;
  } else if ((cond = strcmp(w, p->word)) == 0){
    p->count++; // repeated word
  } else if (cond < 0){
    p->left = addtree(p->left, w);
  } else {
    p->right = addtree(p->right, w);
  }

  return p;
}

int main(){
  struct tnode *root;
  char word[MAXWORD];

  root = NULL;
  while (getword(word, MAXWORD) != EOF){
    if (isalpha(word[0])){
      root = addtree(root, word);
    }
  }
  treePrint(root);
  return 0;
}
