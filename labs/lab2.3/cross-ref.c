#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *dupstr(char *s)
{
  char *p = NULL;

  if(s != NULL)
  {
    p = malloc(strlen(s) + 1);
    if(p)
    {
      strcpy(p, s);
    }
  }

  return p;
}

/* case-insensitive string comparison */
int strcmpIgnoreCase(const char *s, const char *t)
{
  int diff = 0;
  char cs = 0;
  char ct = 0;

  while(diff == 0 && *s != '\0' && *t != '\0')
  {
    cs = tolower((unsigned char)*s);
    ct = tolower((unsigned char)*t);
    if(cs < ct)
    {
      diff = -1;
    }
    else if(cs > ct)
    {
      diff = 1;
    }
    ++s;
    ++t;
  }

  if(diff == 0 && *s != *t)
  {
    if(*s == '\0')
    {
      diff = -1;
    }
    else
    {
      diff = 1;
    }
  }

  return diff;
}


struct linelist
{
  struct linelist *next;
  int line;
};

struct wordtree
{
  char *word;
  struct linelist *firstline;
  struct wordtree *left;
  struct wordtree *right;
};

void printlist(struct linelist *list)
{
  if(list != NULL)
  {
    printlist(list->next);
    printf("%6d ", list->line);
  }
}

void printtree(struct wordtree *node)
{
  if(node != NULL)
  {
    printtree(node->left);
    printf("%18s  ", node->word);
    printlist(node->firstline);
    printf("\n");
    printtree(node->right);
  }
}

struct linelist *addlink(int line)
{
  struct linelist *new = malloc(sizeof *new);
  if(new != NULL)
  {
    new->line = line;
    new->next = NULL;
  }

  return new;
}

void deletelist(struct linelist *listnode)
{
  if(listnode != NULL)
  {
    deletelist(listnode->next);
    free(listnode);
  }
}

void deleteword(struct wordtree **node)
{
  struct wordtree *temp = NULL;
  if(node != NULL)
  {
    if(*node != '\0')
    {
      if((*node)->right != NULL)
      {
        temp = *node;
        deleteword(&temp->right);
      }
      if((*node)->left != NULL)
      {
        temp = *node;
        deleteword(&temp->left);
      }
      if((*node)->word != NULL)
      {
        free((*node)->word);
      }
      if((*node)->firstline != NULL)
      {
        deletelist((*node)->firstline);
      }
      free(*node);
      *node = NULL;
    }
  }
}

struct wordtree *addword(struct wordtree **node, char *word, int line)
{
  struct wordtree *wordloc = NULL;
  struct linelist *newline = NULL;
  struct wordtree *temp = NULL;
  int diff = 0;

  if(node != NULL && word != NULL)
  {
    if(NULL == *node)
    {
      *node = malloc(sizeof **node);
      if(NULL != *node)
      {
        (*node)->left = NULL;
        (*node)->right = NULL;
        (*node)->word = dupstr(word);
        if((*node)->word != NULL)
        {
          (*node)->firstline = addlink(line);
          if((*node)->firstline != NULL)
          {
            wordloc = *node;
          }
        }
      }
    }
    else
    {
      diff = strcmpIgnoreCase((*node)->word, word);
      if(0 == diff)
      {
        newline = addlink(line);
        if(newline != NULL)
        {
          wordloc = *node;
          newline->next = (*node)->firstline;
          (*node)->firstline = newline;
        }
      }
      else if(0 < diff)
      {
        temp = *node;
        wordloc = addword(&temp->left, word, line);
      }
      else
      {
        temp = *node;
        wordloc = addword(&temp->right, word, line);
      }
    }
  }

  if(wordloc == NULL)
  {
    deleteword(node);
  }

  return wordloc;
}

char *char_in_string(char *s, int c)
{
  char *p = NULL;

  /* if there's no data, we'll stop */
  if(s != NULL)
  {
    if(c != '\0')
    {
      while(*s != '\0' && *s != c)
      {
        ++s;
      }
      if(*s == c)
      {
        p = s;
      }
    }
  }

  return p;
}

char *trimWord(char **s, char *delims)
{
  char *p = NULL;
  char *q = NULL;

  if(s != NULL && *s != '\0' && delims != NULL)
  {
    /* pass over leading delimiters */
    while(NULL != char_in_string(delims, **s))
    {
      ++*s;
    }
    if(**s != '\0')
    {
      q = *s + 1;
      p = *s;
      while(*q != '\0' && NULL == char_in_string(delims, *q))
      {
        ++q;
      }

      *s = q + (*q != '\0');
      *q = '\0';
    }
  }

  return p;
}

int isNoiseWord(char *s)
{
  int found = 0;
  int giveup = 0;

  char *list[] =
  {
    "a",
    "about",
    "against",
    "an",
    "and",
    "among",
    "at",
    "be",
    "but",
    "by",
    "during",
    "for",
    "from",
    "he",
    "her",
    "his",
    "I",
    "in",
    "including",
    "into",
    "is",
    "it",
    "its",
    "of",
    "off",
    "on",
    "our",
    "she",
    "so",
    "such",
    "that",
    "the",
    "their",
    "these",
    "they",
    "this",
    "those",
    "to",
    "until",
    "up",
    "was",
    "which",
    "with",
    "you",
    "your",
    "yours"
  };
  int top = sizeof list / sizeof list[0] - 1;

  int bottom = 0;

  int guess = top / 2;

  int diff = 0;

  if(s != NULL)
  {
    while(!found && !giveup)
    {
      diff = strcmpIgnoreCase(list[guess], s);
      if(0 == diff)
      {
        found = 1;
      }
      else if(0 < diff)
      {
        top = guess - 1;
      }
      else
      {
        bottom = guess + 1;
      }
      if(top < bottom)
      {
        giveup = 1;
      }
      else
      {
        guess = (top + bottom) / 2;
      }
    }
  }

  return found;
}

char *getLine(char *s, int n, FILE *fp)
{
  int c = 0;
  int done = 0;
  char *p = s;

  while(!done && --n > 0 && (c = getc(fp)) != EOF)
  {
    if((*p++ = c) == '\n')
    {
      done = 1;
    }
  }

  *p = '\0';

  if(EOF == c && p == s)
  {
    p = NULL;
  }
  else
  {
    p = s;
  }

  return p;
}

#define MAXLINE 8192

int main(int argc, char *argv[])
{
  static char buffer[MAXLINE] = {0};
  char *s = NULL;
  char *word = NULL;
  int line = 0;
  int giveup = 0;
  struct wordtree *tree = NULL;
  FILE *fp;

  if ((fp = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "can′t open %s\n", argv[1]);
    return 1;
  }

  char *delims = " \t\n\r\a\f\v!\"%^&*()_=+{}[]\\|/,.<>:;#~?";

  while(!giveup && getLine(buffer, sizeof buffer, fp) != NULL)
  {
    ++line;
    s = buffer;
    while(!giveup && (word = trimWord(&s, delims)) != NULL)
    {
      if(!isNoiseWord(word))
      {
        if(NULL == addword(&tree, word, line))
        {
          printf("Error adding data into memory. Giving up.\n");
          giveup = 1;
        }
      }
    }
  }

  if(!giveup)
  {
    printf("%18s  Line Numbers\n", "Word");
    printtree(tree);
  }

  deleteword(&tree);

  return 0;
}
