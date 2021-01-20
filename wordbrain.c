#include "wordbrain.h"

int boardSize, wordAmmount, wordPos, happy, funn;
static int length = 0;
int running = 1;
int branch[255];
char word[255], line[255]; 
node root = {'\0', NULL, 0};

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
static char* translate(char letter){
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

//Sjekker ut faktisk lengde(med æøå)
static int lengthCheck(char word[]){
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
static void readTXT(char filename[]){
    FILE* fp = fopen(filename, "r");
    if(!fp){
        printf("Klarte ikke å hente fil. %s\n", filename);
    }
    char line[255];

    while (fgets(line, sizeof(line), fp) != NULL) {
        line[(strlen(line)-1)] = '\0';
        addWord(&root, line);
        //printf("Lagt til: %s\n", line);
    }

    fclose(fp);
}

//Lager et rutenett utifra gitt dimensjoner
static void makeList(){
    board = malloc(sizeof(cell)*boardSize*boardSize);

    //Skriv inn hele ordkartet
    int counter = 0;
    for(int pos = 0; pos <= strlen(line); pos++){
        if((unsigned char) line[pos-1] == 195){
            if((unsigned char) line[pos+counter] == 166){line[--pos] = '1';counter++;}
            if((unsigned char) line[pos+counter] == 184){line[--pos] = '2';counter++;}
            if((unsigned char) line[pos+counter] == 165){line[--pos] = '3';counter++;}
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
static void resetVisited(int cor){
    board[cor].visited = 0;
}

//Gjør alle koordinatene om til visited = 0
static void resetAllVisited(void){
    for(int i = 0; i < (boardSize*boardSize); i++){
        resetVisited(i);
    }
}

/*
//Spør om lengden til ordet
static int initLenth(){
    char buf[30];
    printf("Skriv inn lengden på ordet: ");
    scanf("%d", &length);
            
    snprintf(buf, 30, "--%d--\n", length);
    strcpy(answers.answer[answers.ammount++], buf);

    return length;
}
*/

//Lager ordet utifra koordinatene
static void makeWord(){
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
    word[wordLength] = '\0';
    //printf("%s", word);
    //length -= (i-j);
}

//Sjekker om ordet allerede finnes, ellers legger til i lista
static void checkDuplicates(char text[]){
    for(int i = 0; i < answers.ammount; i++){
        if(!strcmp(text, answers.answer[i]))
            return;
    }
    strcpy(answers.answer[answers.ammount++], text);
    
    strcpy(&allWords[strlen(allWords)], text);
    allWords[strlen(allWords)] = '\r';
    allWords[strlen(allWords)] = '\n';
    //strcpy(allWords, "ape\nape\nape\n");
    //printf("%s\n", text);
}

//Sjekker ut om denne ordet finnes i ordlista
static void checkAnswer(void){
    makeWord();
    //printf("sjekker ord: %s %d\n", word, checkWord(&root, word));
    if(!checkWord(&root, word))
        checkDuplicates(word);

    //printf("ord: %s\n", word);
}

/*
HOVEDALGORITMEN - slår sammen bokstav i et punkt med
alle mulige alternativer. Kaller seg selv med det nye ordet
*/
static void getLetter(int pos, char word[]){
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
static void printBoard(void){
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
static void printAnswers(void){
    for(int i = 0; i < answers.ammount; i++){
        printf("%s", answers.answer[i]);
    }
}

void solvePuzzle(char* myWord, int wordLength){
    length = wordLength;
    boardSize = sqrt(strlen(myWord));
    strcpy(line, myWord);

    answers.ammount = 0;
    readTXT("ordlisten.txt");
    makeList();

    resetAllVisited();
    //printBoard();
    for(int i = 0; i < (boardSize*boardSize); ++i){
        wordPos = 1;
        branch[0] = i;
        //printf("Start: %d, bokstav: %c\n", i, board[i].letter);
        getLetter(i, word);
        resetAllVisited();
    }   
    
}