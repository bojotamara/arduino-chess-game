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
bool validateMove(int piecetomove) {
	bool valid = false;

	switch (piecetomove) {
		case W_PAWN:
			// valid if pawn is moved forward on an empty space
			if (board[selectedY][selectedX] == EMPTY && selectedX == chosenX && selectedY == chosenY - 1){
				valid = true;
			}
			//valid if diagonal, and pawn eats opponent
			else if ((selectedX == chosenX -1 || selectedX == chosenX + 1) && selectedY == chosenY - 1 && board[selectedY][selectedX] < 0 ) {
				valid = true;
			}
			break;
		case B_PAWN:
			// valid if pawn is moved forward on an empty space
			if (board[selectedY][selectedX] == EMPTY && selectedX == chosenX && selectedY == chosenY + 1){
				valid = true;
			}
			//valid if diagonal, and pawn eats opponent
			else if ((selectedX == chosenX -1 || selectedX == chosenX + 1) && selectedY == chosenY + 1 && board[selectedY][selectedX] > 0 ) {
				valid = true;
			}
			break;
		default : valid = true; // just so when working on this, the other moves will always be valid
	}

	return valid;
}
