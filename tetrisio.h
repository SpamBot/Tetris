#include<stdio.h>


void* initgfx(){
   return 0; 
}

char getinput(){
    char out='a';
    switch(getchar()){
        case 'a': out='l'; break;
        case 's': out='d'; break;
        case 'd': out='r'; break;
        case ' ': out='R'; break;
        default: break;
    }
    return out;
}

void drawgfx(char (*board)[10], int score, int hiscore, int next_piece_num){
    char next_piece;
    switch(next_piece_num){
        case 0: next_piece='#'; break;
        case 1: next_piece='I'; break;
        case 2: next_piece='L'; break;
        case 3: next_piece='J'; break;
        case 4: next_piece='Z'; break;
        case 5: next_piece='5'; break;
        case 6: next_piece='T'; break;
        default: next_piece=' '; break;
    }
    for(int i=0; i<100; i++)printf("\n");
    for(int i=0; i<20; i++){
        for(int k=0; k<30; k++) printf(" ");
        printf("#");
        for(int j=0; j<10; j++)
            if(board[i][j]==1){
                printf("1");
            }
            else if(board[i][j]==2){
                printf("2");
            }
            else printf(" ");
         printf("#");
         if(i==1)printf("    next: %c", next_piece);
         if(i==10)printf("    score: %d", score);
         if(i==12)printf("    hi-score: %d", hiscore);
         printf("\n");
     }
     for(int k=0; k<30; k++) printf(" ");
     for(int j=0; j<11; j++)printf("#");
}
