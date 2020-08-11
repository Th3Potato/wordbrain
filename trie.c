#include "trie.h"

node *createNode(char letter){
    node *new = malloc(sizeof(node));
    if(!new){
        printf("Klarte ikke å malloce\n");
    }
    new->c = letter;
    new->list = NULL;
    new->size = 0;

    return new;
}


void addNode(node *pnode, char letter){
    node *new = createNode(letter);

    pnode->list = realloc(pnode->list, (sizeof(node *) * (pnode->size+1)));
    if(!(pnode->list)){
        printf("Klarte ikke å realloce\n");
    }
    pnode->list[pnode->size] = new;
    pnode->size++; 
}

int checkList(node *pnode, char letter){
    if(pnode->size == 0 || pnode->list == NULL)
        return -1;

    for(int i = 0; i < pnode->size; ++i){
        if(pnode->list[i]->c == letter)
            return i;
    }

    return -1;
}

void addWord(node *pnode, char *text){
    for(int i = 0; i <= strlen(text); ++i){
        int pos = checkList(pnode, text[i]);
        if(pos == -1){
            addNode(pnode, text[i]);
            pnode = pnode->list[pnode->size-1];
        }else{
            pnode = pnode->list[pos];
        }
    }
    //printf("Word added\n");
}

int checkWord(node *pnode, const char *text){
    for(int i = 0; i <= strlen(text); ++i){

        int pos = checkList(pnode, text[i]);

        if(pos == -1)
            return -1;
        
        pnode = pnode->list[pos];
    }
    return 0;
}
