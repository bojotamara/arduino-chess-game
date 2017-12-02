#include <Arduino.h>
#include "chessfunctions.h"

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

/*
Function that returns a boolean based on whether or not a move is valid for
a specified piece
*/
bool validateMove(int piecetomove, int selX, int selY) {
	bool valid = false;

	switch (piecetomove) {
		case W_PAWN:
			// valid if pawn is moved forward on an empty space
			if (board[selY][selX] == EMPTY && selX == chosenX && selY == chosenY - 1){
				valid = true;
			}
			//valid if diagonal, and pawn eats opponent
			else if ((selX == chosenX -1 || selX == chosenX + 1) && selY == chosenY - 1 && board[selY][selX] < 0 ) {
				valid = true;
			}
			break;
		case B_PAWN:
			// valid if pawn is moved forward on an empty space
			if (board[selY][selX] == EMPTY && selX == chosenX && selY == chosenY + 1){
				valid = true;
			}
			//valid if diagonal, and pawn eats opponent
			else if ((selX == chosenX -1 || selX == chosenX + 1) && selY == chosenY + 1 && board[selY][selX] > 0 ) {
				valid = true;
			}
			break;
		default : valid = true; // just so when working on this, the other moves will always be valid
	}

	return valid;
}

void highlightValid(int pieceToMove){
  for (int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      Serial.println(validateMove(pieceToMove, i, j));
      if(validateMove(pieceToMove, i, j)){
        highlightSquare(i,j,0x600F);
      }
    }
  }

}
