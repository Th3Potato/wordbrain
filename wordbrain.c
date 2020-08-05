#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef LITEN 
#define listName "ordliste.txt"
#define listLength 65422
#endif

#ifdef STOR 
#define listName "ordlisten.txt"
#define listLength 622115
#endif

#ifdef ENG 
#define listName "ordliste_E.txt"
#define listLength 84099
#endif

int boardSize, length, wordPos, happy, funn;
int running = 1;
int branch[255];
char word[255]; 
char wordList[37337][255];
//char wordList[10][listLength][255];

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

//gjør om '1', '2', '3' til 'æ', 'ø', 'å'
char* translate(char letter){
    char *newLetter;

    newLetter = malloc(sizeof(char)*2);
    memset(newLetter, 0, 3);

    switch(letter){
        case '1':
            newLetter[0] = 195;
            newLetter[1] = 166;
            return newLetter;
        case '2':
            newLetter[0] = 195;
            newLetter[1] = 184;
            return newLetter;
        case '3':
            newLetter[0] = 195;
            newLetter[1] = 165;
            return newLetter;
        default:
            free(newLetter);
            return NULL;
    }
}

int lengthCheck(char word[]){
    int checkLength = 0;
    for(int pos = 0; pos < strlen(word); pos++){
        if((unsigned char) word[pos-1] == 195){
            checkLength--;
        }
        checkLength++;
    }
    checkLength--;
    //printf("lengde: %d\n", checkLength);
    return checkLength;
}

//åpner ordlista og fyller inn hvert ord i en 2-dimensjonal liste
void readTXT(char filename[]){
    FILE* fp = fopen(filename, "r");
    if(!fp){
        printf("Klarte ikke å hente fil. %s\n", filename);
    }
    char line[255];
    int i = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        //printf("%s\n", line);
        strcpy(wordList[i], line);
        i++;
    }

    fclose(fp);
}

//Lager et rutenett utifra gitt dimensjoner
void makeList(void){
    printf("Skriv inn dimmensjonene på rutenettet: ");
    scanf("%d", &boardSize);

    board = malloc(sizeof(cell)*boardSize*boardSize);

    //Skriv inn hele ordkartet
    char line[255];
    printf("Skriv inn hele ordetkartet i en string: \n");
    scanf("%s", line);
    int counter = 0;
    for(int pos = 0; pos <= strlen(line); pos++){
        if(line[pos-1] == 195){
            if(line[pos] == 166){line[--pos] = '1';counter++;}
            if(line[pos] == 184){line[--pos] = '2';counter++;}
            if(line[pos] == 165){line[--pos] = '3';counter++;}
        }else{
            line[pos] = line[pos+counter];
        }
    
    }

    for(int i = 0; i < (boardSize*boardSize); ++i){
        board[i].letter = line[i];
    }   

    /*
    //Skriv inn bokstav for bokstav
    for(int i = 0; i < (boardSize*boardSize); ++i){
        printf("Skriv inn bokstav på: x = %d, y = %d\n", i%boardSize, i/boardSize);
        scanf("%s", line);
        board[i].letter = line[0];
    }  
    */
}

//Gjør koordinat i bestemt koor om til visited = 0
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
    char buf[30];
    printf("Skriv inn lengden på ordet: ");
    scanf("%d", &length);

    char buffer[10];
    if(length >= 10){
        snprintf(buffer, 10, "%d%s.txt", length, "word");
    }else{
        buffer = "word.txt";
    }
    readTXT(buffer);
            
    snprintf(buf, 30, "--%d--\n", length);
    strcpy(answers.answer[answers.ammount++], buf);

    return length;
}

//Lager ordet utifra koordinatene
void makeWord(){
    int i = 0; //posisjonen i ordet(påvirket av æøå)
    int j = 0; //orginale posisjonen
    int wordLength = length;
    for(; i < wordLength; i++){
        if(translate(board[branch[j]].letter) != NULL){
            char* newLetter = translate(board[branch[j]].letter);
            word[i] = newLetter[0];
            wordLength++;i++;
            word[i] = newLetter[1];
        }else{
            word[i] = board[branch[j]].letter;
        }
        j++;
    }
    word[wordLength] = '\n';
    word[wordLength+1] = '\0';
    //printf("%s", word);
    //length -= (i-j);
}

//Sjekker om ordet allerede finnes, ellers legger til i lista
void checkDuplicates(char text[]){
    for(int i = 0; i < answers.ammount; i++){
        if(!strcmp(text, answers.answer[i]))
            return;
    }
    strcpy(answers.answer[answers.ammount++], text);
    printf("%s", text);
}

//Sjekker ut om denne ordet finnes i ordlista
void checkAnswer(void){
    makeWord();
    //printf("ord: %s\n", word);
    for(int i = 0; i <= 37337; i++){
        if(!strcmp(word, wordList[i])){
            funn++;
            //printf("Forslag til ord: %s\n", word);
            checkDuplicates(word);
            break;
        }
    }
}

/*
HOVEDALGORITMEN - slår sammen bokstav i et punkt med
alle mulige alternativer. Kaller seg selv med det nye ordet
*/
void getLetter(int pos, char word[]){
    int x = -1;
    int y = -1;
    int newPos = pos;

    board[pos].visited = 1;
    //sjekker om ordet passer
    if(wordPos == length){
        checkAnswer();
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
        //printf("%c ", board[i].letter);
        
        if(translate(board[i].letter) == NULL){
            printf("%c ", board[i].letter);
        }else{
            printf("%s ", translate(board[i].letter));
        }
        
    }
    printf("\n\n");
}

//printer ut alle svarene
void printAnswers(void){
    for(int i = 0; i < answers.ammount; i++){
        printf("%s", answers.answer[i]);
    }
}

int main(void){
    answers.ammount = 0;
    //readTXT();
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
            //printAnswers();
            //printf("Antall forslag: %d\n", funn);
            printf("\nEr du fornøyd med ordene du fikk? (0/1) ");
            scanf("%d", &happy);
            if(happy){
                running = 0;
            }
        }
    }
    
    return 0;
}