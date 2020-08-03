#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//ordliste: 65422, ordlisten: 622115, ordliste_E: 84099
#define listName "ordlisten.txt"
#define listLength 622115

int boardSize, length, wordPos, happy;;
int running = 1;
int branch[255];
char tjuesju = 'æ'; char tjueatte = 'ø'; char tjueni = 'å';
char word[255], wordList[listLength][255];

typedef struct{
    int visited;
    char letter;
}cell;

typedef struct{
    int ammount;
    char answer[255][255];
}answerList;

cell* board;
answerList answers;

//åpner ordlista og fyller inn hvert ord i en 2-dimensjonal liste
void readTXT(void){
    FILE* fp = fopen(listName, "r");
    char line[255];
    int i = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        strcpy(wordList[i], line);
        i++;
    }

    fclose(fp);
}

void writeTXT(void){
    FILE* fp = fopen("ordlisten.txt", "a");

    fclose(fp);
}

//Lager et rutenett utifra gitt dimensjoner
void makeList(void){
    printf("Skriv inn dimmensjonene på rutenettet: ");
    scanf("%d", &boardSize);
    char line[100];

    board = malloc(sizeof(cell)*boardSize*boardSize);

    /*
    //Skriv inn hele ordkartet
    printf("Skriv inn hele ordetkartet i en string: \n");
    scanf("%s", line);
    for(int i = 0; i < (boardSize*boardSize); ++i){
        board[i].letter = line[i];
    }
    */
    
    //Skriv inn bokstav for bokstav
    for(int i = 0; i < (boardSize*boardSize); ++i){
        printf("Skriv inn bokstav på: x = %d, y = %d\n", i%boardSize, i/boardSize);
        scanf("%s", line);
        if(!strcmp(line, "tjuesju")){
            board[i].letter = tjuesju;
        }
        else if(!strcmp(line, "tjueatte")){
            board[i].letter = tjueatte;
        }
        else if(!strcmp(line, "tjueni")){
            board[i].letter = tjueni;
        }
        else{
            board[i].letter = line[0];
            }
    }  
    
}

//Gjør koordinat i bestemt pos om til visited = 0
void resetVisited(int cor){
    board[cor].visited = 0;
}

//Gjør alle koordinatene om til visited = 0
void resetAllVisited(void){
    for(int i = 0; i < (boardSize*boardSize); i++){
        resetVisited(i);
    }
}

//Spør om lengden til ordet
int initLenth(){
    printf("Skriv inn lengden på ordet: ");
    scanf("%d", &length);
    return length;
}

//Lager ordet utifra koordinatene
void makeWord(){
    for(int i = 0; i < length; i++){
        word[i] = board[branch[i]].letter;
    }
    word[length] = '\n';
    //printf("%s", word);
}

void checkDuplicates(char text[]){
    for(int i = 0; i < answers.ammount; i++){
        if(!strcmp(text, answers.answer[i]))
            return;
    }
    strcpy(answers.answer[answers.ammount++], text);
}

//Sjekker ut om denne ordet finnes i ordlista
void checkAnswer(){
    makeWord();
    for(int i = 0; i <= listLength; i++){
        if(!strcmp(word, wordList[i])){
            //printf("Forslag til ord: %s\n", word);
            checkDuplicates(word);
            break;
        }
    }
}

void getLetter(int pos, char word[]){
    int x = -1;
    int y = -1;
    int newPos = pos;

    board[pos].visited = 1;
    //sjekker om ordet passer
    if(wordPos == length){
        checkAnswer(branch);
    }

    while(y < 2){   
        newPos = pos+(y*boardSize+x);
        if((pos%boardSize == boardSize-1 && x == 1) || (pos%boardSize == 0 && x == -1)){
            newPos = pos+(y*boardSize);
        }
        if(newPos >= 0 && newPos < (boardSize*boardSize) && board[newPos].visited == 0 && wordPos < length){
            //printf("koor: %d, bokstav: %c\n", newPos, board[newPos].letter);
            branch[wordPos] = newPos;
            board[newPos].visited = 1;
            wordPos++;

            getLetter(newPos, word);
        }
        x++;
        if(x == 2){
            x = -1;
            y++;
        }
    }
    resetVisited(pos);
    wordPos--;
}

//printer ut koordinatsystemet
void printBoard(void){
        for(int i = 0; i < (boardSize*boardSize); ++i){
        if(!(i%boardSize)) {printf("\n");}
        printf("%c ", board[i].letter);
    }
    printf("\n\n");
}

void printAnswers(void){
    for(int i = 0; i < answers.ammount; i++){
        printf("%s", answers.answer[i]);
    }
}

int main(void){
    answers.ammount = 0;
    readTXT();
    makeList();
    while(running){
        resetAllVisited();
        printBoard();
        initLenth();
        for(int i = 0; i < (boardSize*boardSize); ++i){
            wordPos = 1;
            branch[0] = i;
            //printf("Start: %d, bokstav: %c\n", i, board[i].letter);
            getLetter(i, word);
            resetAllVisited();
        }   

        if(answers.ammount){
            printAnswers();
            printf("\nEr du fornøyd med ordene du fikk? (0/1) ");
            scanf("%d", &happy);
            if(happy){
                running = 0;
            }
        }else{
            printf("Fant ingen ord. Prøv på nytt.\n");
        }
    }
    
    return 0;
}