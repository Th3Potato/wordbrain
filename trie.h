#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct snode node;

struct snode{
    char c;
    node **list;
    int size;
};

node *createNode(char);
void addNode(node *, char);
int checkList(node *, char);
void addWord(node *, char *);
int checkWord(node *, const char *);