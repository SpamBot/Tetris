#include "tetris.h"

int main(int argc, char** argv){
    Tetris t;
    t.clr_hi_score();
    for(;;)t.play();
}
