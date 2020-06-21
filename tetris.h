/*
Tetris
based on: http://javilop.com/gamedev/tetris-tutorial-in-c-platform-independent-focused-in-game-logic-for-beginners/
*/
#include<cstdlib>//srand(), rand()
#include<ctime>//time()
#include "tetrisio.h"//drawgfx(), getinput()

#define BOARD_h 20
#define BOARD_w 10

#define DRAW_TETRIS_GFX drawgfx(&board[0], score, hi_score, next_piece.getType());


int tetraminoes[7/*types*/][3/*blocks, center is at {0, 0}*/][2/*x, y*/]={
    {//x+: down;   y+: right (it's array based)
        {0, 1},{1, 0},{1, 1}//square
        /*
        c x
        x x
        */
    },
    {
        {0, -1},{0, 1},{0, 2}//I
        /*
        x c x x
        */
    },
    {
        {-1, 0},{1, 0},{1, 1}//L
        /*
        x
        c
        x x
        */
    },
    {
        {-1, 0},{1, 0},{1, -1}//L mirrored
        /*
          x
          c
        x x
        */
    },
    {
        {0, 1},{-1, 1},{1, 0}//Z
        /*
          x
        c x
        x
        */
    },
    {
        {-1, 0},{0, 1},{1, 1}//Z mirrored
        /*
        x
        c x
          x
        */
    },
    {
        {-1, 0},{0, 1},{1, 0}//T
        /*
        x
        c x
        x
        */
    }
};

class Tpiece{
    private:
        int type, center_x, center_y, blocks[3][2];
    
    public:
        void initpiece(int t){
            type=t; center_x=1; center_y=BOARD_w/2;
            for(int i=0; i<3; i++){
                blocks[i][0]=tetraminoes[t][i][0];
                blocks[i][1]=tetraminoes[t][i][1];
            }
        }
        
        char getType(){return type;}
        
        int *getBlock(int i){return &blocks[i%3][0];}
        
        char getX(){return center_x;}
        char getY(){return center_y;}
        
        void rotate_c(){//clockwise
        //(1,0)->(0,-1)->(-1,0)->(0,1)
        //(1,1)->(1,-1)->(-1,-1)->(-1,1)
            int tmp;
            for(int i=0; i<3; i++){//x=y; y=-x;
                tmp=blocks[i][0];
                blocks[i][0]=blocks[i][1];
                blocks[i][1]=-tmp;
            }
        }
        void rotate_cc(){
            int tmp;
            for(int i=0; i<3; i++){//x=y; y=-x;
                tmp=blocks[i][0];
                blocks[i][0]=-blocks[i][1];
                blocks[i][1]=tmp;
            }
        }
        void fall(){center_x++;}
        void rise(){center_x--;}
        void left(){center_y--;}
        void right(){center_y++;}
};

class Tetris{
    private:
        int score, hi_score;
        char board[BOARD_h][BOARD_w];
        enum block_types{empty, falling, fallen};
        Tpiece curr_piece, next_piece;
        
        void clr_line(int y){
            for(int i=y; i>0; i--)
                for(int j=0; j<BOARD_w; j++)board[i][j]=board[i-1][j];
            score++;
        }
        
        void clr_board(){
            for(int i=0; i<BOARD_h; i++)
                for(int j=0; j<BOARD_w; j++)board[i][j]=empty;
        }
        void fill_piece(Tpiece *p, char fill){
            int *block_ptr, x=p->getX(), y=p->getY();
            board[x][y]=fill;
            for(int i=0; i<3; i++){
                block_ptr=p->getBlock(i);
                board[x+block_ptr[0]][y+block_ptr[1]]=fill;
            }
        }
    
    public:
        void clr_hi_score(){hi_score=0;}
        
        void fade_in(char fill){//just a visual effect, not necessary for the actual game
            int x=1, y=1, dx=0, dy=1;
            char over_board[BOARD_h+2][BOARD_w+2];
            for(int i=0; i<BOARD_h+2; i++)
                for(int j=0; j<BOARD_w+2; j++)over_board[i][j]=fill+1;
            for(int j=0; j<BOARD_w+2; j++){over_board[0][j]=fill; over_board[BOARD_h+1][j]=fill;}
            for(int i=0; i<BOARD_h+2; i++){over_board[i][0]=fill; over_board[i][BOARD_w+1]=fill;}
            over_board[x][y]=fill; board[0][0]=fill;
            for(;;){
                if(over_board[x+dx][y+dy]!=fill){
                    x+=dx; y+=dy; board[x-1][y-1]=fill; over_board[x][y]=fill;
                    DRAW_TETRIS_GFX
                    clock_t target = clock() + 6250;
                    while (clock() < target){ }
                }
                else{
                    if(dx==0 && dy==1){dx=1; dy=0;}
                    else if(dx==1 && dy==0){dx=0; dy=-1;}
                    else if(dx==0 && dy==-1){dx=-1; dy=0;}
                    else if(dx==-1 && dy==0){dx=0; dy=1;}
                    if(over_board[x+dx][y+dy]==fill)break;
                }
            }
        }
        
        int gameOver(){
            int out=0;
            for(int j=0; j<BOARD_w; j++)if(board[0][j]==fallen){out=1; break;}
            return out;
        }
        
        void scan_board(){
            int t;
            for(int i=0; i<BOARD_h; i++){
                t=0;
                for(int j=0; j<BOARD_w; j++)if(board[i][j]==fallen)t++;
                if(t==BOARD_w)clr_line(i);
            }
        }
        int bannedMove(Tpiece *p){
            int out=0, x=p->getX(), y=p->getY();
            int *block_ptr;
            if(x<0 || x>=BOARD_h || y<0 || y>=BOARD_w || board[x][y]==fallen)out=1;
            else for(int i=0; i<3; i++){
                block_ptr=p->getBlock(i);
                x+=block_ptr[0]; y+=block_ptr[1];
                if(x<0 || x>=BOARD_h || y<0 || y>=BOARD_w || board[x][y]==fallen){out=1; break;}
                x-=block_ptr[0]; y-=block_ptr[1];
            }
            return out;
        }
        void play(){
            //void *canvas=initgfx();//tetrisio.h
            clr_board(); score=0;
            std::srand(time(NULL));
            next_piece.initpiece(std::rand() % 7);
            fade_in(falling); clr_board();//delete this line if you are short on memory
            DRAW_TETRIS_GFX
            //next_piece.initpiece(0);
            for(;;){
                curr_piece=next_piece;
                next_piece.initpiece(std::rand() % 7);
                //next_piece.initpiece(1);
                for(;;){
                    fill_piece(&curr_piece, empty);
                    curr_piece.fall();
                    if(bannedMove(&curr_piece)!=0){curr_piece.rise(); break;}
                    switch(getinput()){//tetrisio.h
                        case 'l': {
                                   curr_piece.left();
                                   if(bannedMove(&curr_piece)!=0)curr_piece.right();
                                   break;
                                  }
                        case 'r': {
                                   curr_piece.right();
                                   if(bannedMove(&curr_piece)!=0)curr_piece.left();
                                   break;
                                  }
                        case 'R': {
                                   curr_piece.rotate_c();
                                   if(bannedMove(&curr_piece)!=0)curr_piece.rotate_cc();
                                   break;
                                  }
                        case 'd': {
                                   while(bannedMove(&curr_piece)==0)curr_piece.fall();
                                   curr_piece.rise();
                                   break;
                                  }
                        default: break;
                    }
                    fill_piece(&curr_piece, falling);
                    //drawgfx(canvas, &board[0]);//tetrisio.h
                    DRAW_TETRIS_GFX
                }
                fill_piece(&curr_piece, fallen);
                scan_board();
                if(gameOver()==1)break;
            }
            if(score>hi_score)hi_score=score;
            DRAW_TETRIS_GFX
            fade_in(fallen); clr_board(); DRAW_TETRIS_GFX//delete this line if you are short on memory
        }
};
