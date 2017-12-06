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
      //Serial.print("Position: "); Serial.print(x); Serial.print(" "); Serial.println(y);
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

bool castling(int x, int y){
  //castling will be handled in a weird way.
  //If the user wishes to castle, he/she must select the king, and then
  //highlight the rook to be castled with. If this is possible, the switch will
  //happen automatically

  // Serial.print(x);Serial.print(" "); Serial.print(y);
  // Serial.println(p1_leftRookmoved);
  // Serial.println();

  switch (currentplayer){
    case 1:
      if ( x==0 && y==7 && !p1_leftRookmoved && !p1_kingMoved ){//left rook
        if(board[y][3]==EMPTY && board[y][2]==EMPTY){
          //need to check if king would be in check along the squares
          specialmovepiece(4,7,2,7,W_KING);
          specialmovepiece(0,7,3,7,W_ROOK);
          return 1;
        }
      }
      else if ( x==7 && y==7 && !p1_rightRookmoved && !p1_kingMoved ){//right rook
        if(board[y][5]==EMPTY && board[y][6]==EMPTY){
          //need to check if king would be in check along the squares
          specialmovepiece(4,7,6,7,W_KING);
          specialmovepiece(7,7,5,7,W_ROOK);
          return 1;
        }
      }
    break;

    case 2:
      if ( x==0 && y==0 && !p2_leftRookmoved && !p2_kingMoved ){//left rook
        if(board[y][2]==EMPTY && board[y][3]==EMPTY){
          //need to check if king would be in check along the squares
          specialmovepiece(4,0,2,0,B_KING);
          specialmovepiece(0,0,3,0,B_ROOK);
          return 1;
        }
      }
      else if ( x==7 && y==0 && !p2_rightRookmoved && !p2_kingMoved ){//right rook
        if(board[y][5]==EMPTY && board[y][6]==EMPTY){
          //need to check if king would be in check along the squares
          specialmovepiece(4,0,6,0,B_KING);
          specialmovepiece(7,0,5,0,B_ROOK);
          return 1;
        }
      }
    break;
  }


  return 0;
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
      if(castling(x,y)){
        dispTips("castled");
        delay(2000);
        return 1;
      }
    break;

    case B_KING:
      if(castling(x,y)){
        dispTips("castled");
        delay(2000);
        return 1;
      }
    break;

    // case B_KING:
    // break;

  }
  return 0;
}
