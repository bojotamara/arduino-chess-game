#include <Arduino.h>
#include "chessfunctions.h"
#include "validmoves.h"

#define EMPTY 0

#define W_PAWN 1
#define W_ROOK 2
#define W_KNIGHT 3
#define W_BISHOP 4
#define W_KING 5
#define W_QUEEN 6

#define B_PAWN -1
#define B_ROOK -2
#define B_KNIGHT -3
#define B_BISHOP -4
#define B_KING -5
#define B_QUEEN -6


void promote_to_Queen(int x, int y){
  switch(currentplayer){

    case 1:
      board[y][x] = W_QUEEN;
      drawPiece(x,y,W_QUEEN)
    break;

    case 2:
      board[y][x]=-B_QUEEN;
      drawPiece(x,y,B_QUEEN);
    break;
  }
}
