#include <Arduino.h>
#include "chessfunctions.h"
#include "validmoves.h"
#include "specialcases.h"

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
      drawPiece(x,y,W_QUEEN);
      Serial.print("Position: "); Serial.print(x); Serial.print(" "); Serial.println(y);
    break;

    case 2:
      board[y][x] = B_QUEEN;
      drawPiece(x,y,B_QUEEN);
    break;
  }
}

void en_passant(){
  ;//TODO
}

void specialmovepiece(int oldx, int oldy, int x, int y, int piece){
  //function based on movePiece in chessfunctions.cpp but modified to
  //work for the purpose of castling and other special functions
  	board[oldy][oldx] = EMPTY;
  	board[y][x] = piece;
  	emptySquare(oldx,oldy);
  	drawPiece(x,y,piece);
}

void castling(int x, int y){
  //castling will be handled in a weird way.
  //If the user wishes to castle, he/she must select the king, and then
  //highlight the rook to be castled with. If this is possible, the switch will
  //happen automatically

  if ( x==0 && y==7 && p1_leftRookmoved==0 ){
    if(board[y][x-1]==EMPTY && board[y][x-2]==EMPTY){
      //need to check if king would be in check along the squares
      specialmovepiece(4,7,x-2,y,W_KING);
      specialmovepiece(0,7,x-1,y,W_ROOK);
    }
  }

}

bool checkSpecialcases(int x, int y, int piece){
  //input args x,y are the square that the piece will be moved to

  //returns a boolean, to let function in chessfunctions.cpp know that
  //the movment of pieces has already been taken care of here.

  // Serial.print(piece); Serial.print(" "); Serial.print(x);
  // Serial.print(" "); Serial.println(y);
  // Serial.println();
  switch (piece){
    case W_PAWN:
      if(y==0){
        promote_to_Queen(x,y);
        dispTips("promotion");
        delay(1000);
        return 1;
      }
    break;

    case B_PAWN:
      if(y==7){
        promote_to_Queen(x,y);
        dispTips("promotion");
        delay(1000);
        return 1;
      }
    break;

    case W_KING:
      castling(x,y);
      dispTips("castled");
      delay(2000);
      return 1;
    break;

    case B_KING:
    break;

  }
  return 0;
}
