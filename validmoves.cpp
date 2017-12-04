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
		//need to work on pawns moving 2 spaces on first turn, and leater en passant
		case W_PAWN:
			// valid if pawn is moved forward on an empty space
			if (board[selY][selX] == EMPTY && selX == chosenX){
				//normal move forward
				if (selY == chosenY -1) {
					valid = true;
				}
				//2 spaces at the beginning
				else if (selY == chosenY - 2 && chosenY == 6 && board[selY+1][selX] == EMPTY) {
					valid = true;
				}
			}
			//valid if diagonal, and pawn eats opponent
			else if ((selX == chosenX -1 || selX == chosenX + 1) && selY == chosenY - 1 && board[selY][selX] < 0 ) {
				valid = true;
			}

			break;
		case B_PAWN:
			// valid if pawn is moved forward on an empty space
			if (board[selY][selX] == EMPTY && selX == chosenX){
				//normal move forward
				if (selY == chosenY + 1) {
					valid = true;
				}
				//2 spaces at the beginning
				else if (selY == chosenY + 2 && chosenY == 1 && board[selY-1][selX] == EMPTY) {
					valid = true;
				}
			}
			//valid if diagonal, and pawn eats opponent
			else if ((selX == chosenX -1 || selX == chosenX + 1) && selY == chosenY + 1 && board[selY][selX] > 0 ) {
				valid = true;
			}
			break;

		//need to work on killing opponent, and checking for pieces blocking path
		case W_ROOK:
			//valid if moved horizontally or vertically but not both
			if (selX==chosenX || selY==chosenY){
				valid = true;
			}
			break;

		case B_ROOK:
			//valid if moved horizontally or vertically but not both
			if (selX==chosenX || selY==chosenY){
				valid = true;
			}
			break;

		case W_KNIGHT:
			//valid for 2 blocks up/down and 1 to the right/left
			if(selX==chosenX+1 && selY==chosenY+2){
				valid =true;
			}
			else if(selX==chosenX+1 && selY==chosenY-2){
				valid =true;
			}
			else if(selX==chosenX-1 && selY==chosenY+2){
				valid =true;
			}
			else if(selX==chosenX-1 && selY==chosenY-2){
				valid =true;
			}

			//valid for 2 blocks left/right and 1 up/down
			if(selY==chosenY+1 && selX==chosenX+2){
				valid =true;
			}
			else if(selY==chosenY+1 && selX==chosenX-2){
				valid =true;
			}
			else if(selY==chosenY-1 && selX==chosenX+2){
				valid =true;
			}
			else if(selY==chosenY-1 && selX==chosenX-2){
				valid =true;
			}

			break;

		case B_KNIGHT:
			//valid for 2 blocks up/down and 1 to the right/left
			if(selX==chosenX+1 && selY==chosenY+2){
				valid =true;
			}
			else if(selX==chosenX+1 && selY==chosenY-2){
				valid =true;
			}
			else if(selX==chosenX-1 && selY==chosenY+2){
				valid =true;
			}
			else if(selX==chosenX-1 && selY==chosenY-2){
				valid =true;
			}

			//valid for 2 blocks left/right and 1 up/down
			if(selY==chosenY+1 && selX==chosenX+2){
				valid =true;
			}
			else if(selY==chosenY+1 && selX==chosenX-2){
				valid =true;
			}
			else if(selY==chosenY-1 && selX==chosenX+2){
				valid =true;
			}
			else if(selY==chosenY-1 && selX==chosenX-2){
				valid =true;
			}

			break;

		case W_BISHOP:
			//diagonals, as long as square is empty and path is unobstructed
			for(int i=-7; i<8;i++){

				if(i==0) continue;//if i is 0, skip loop, because otherweise bishop can move up/down/left/right

				if(selX==chosenX+i && selY==chosenY+i || selX==chosenX-i && selY == chosenY+i){
					Serial.print("i:"); Serial.print(i);
					Serial.print(" x:"); Serial.print(selX);
					Serial.print(" y:"); Serial.println(selY);
					valid=true;
					break;
				}
			}


		case B_BISHOP:
			//diagonals, as long as square is empty and path is unobstructed

			for(int i=-7; i<8;i++){
				if(i==0) continue;//if i is 0, skip loop
				if(selX==chosenX+i && selY==chosenY+i || selX==chosenX-i && selY == chosenY+i){
					valid=true;
					break;
				}
			}


		case W_KING:
			//valid for 2 blocks up/down and 1 to the right/left
			if(abs(selX-chosenX)<=1 && abs(selY-chosenY)<=1 && board[selY][selX] <= 0){
				valid=true;
			}
			break;

		case B_KING:
			//valid for 2 blocks up/down and 1 to the right/left
			if(abs(selX-chosenX)<=1 && abs(selY-chosenY)<=1 && board[selY][selX] >=0){
				valid=true;
			}
			break;

		case W_QUEEN:
		//combination of bishop and rook
			if (selX==chosenX || selY==chosenY){
				valid = true;
				break;
			}
			for(int i=-7; i<8;i++){
				if(selX==chosenX+i && selY==chosenY+i || selX==chosenX-i && selY == chosenY+i){
					valid=true;
					break;
				}
			}

			case B_QUEEN:
				if (selX==chosenX || selY==chosenY){
					valid = true;
					break;
				}
				for(int i=-7; i<8;i++){
					if(selX==chosenX+i && selY==chosenY+i || selX==chosenX-i && selY == chosenY+i){
						valid=true;
						break;
					}
				}
	}

	//cant eat your own pieces
	if (currentplayer == 1 && board[selY][selX] > 0) {
		valid = false;
	}
	else if ( currentplayer == 2 && board[selY][selX] < 0) {
		valid = false;
	}

	return valid;
}

bool checkObstruction(int piece, int selX, int selY) {
	bool valid = true;

	// checking the obstruction for vertical, upward lines of attack
	if ( (abs(piece) == W_QUEEN || abs(piece) == W_ROOK) && selX == chosenX && selY < chosenY) {
		for (int i = chosenY-1; i > selY; i--) {
			if (board[i][chosenX] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// checking the obstruction for vertical, downward lines of attack
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_ROOK) && selX == chosenX && selY > chosenY) {
		for (int i = chosenY+1; i < selY; i++) {
			if (board[i][chosenX] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// horizontal, left lines of attack
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_ROOK) && selY == chosenY && selX < chosenX) {
		for (int i = chosenX-1; i > selX; i--) {
			if (board[chosenY][i] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// horizontal, right lines of attack
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_ROOK) && selY == chosenY && selX > chosenX) {
		for (int i = chosenX+1; i < selX; i++) {
			if (board[chosenY][i] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// diagonal, top right
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_BISHOP) && selX > chosenX && selY < chosenY) {
		int y = chosenY;
		for (int i=chosenX + 1; i< selX;i++){
			y -= 1;
			if (board[y][i] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// diagonal, bottom left
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_BISHOP) && selX < chosenX && selY > chosenY) {
		int y = chosenY;
		for (int i=chosenX - 1; i > selX;i--){
			y += 1;
			if (board[y][i] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// diagonal, top left
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_BISHOP) && selX < chosenX && selY < chosenY) {
		int y = chosenY;
		for (int i=chosenX - 1; i > selX;i--){
			y -= 1;
			if (board[y][i] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// diagonal, bottom right
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_BISHOP) && selX > chosenX && selY > chosenY) {
		int y = chosenY;
		for (int i=chosenX + 1; i < selX;i++){
			y += 1;
			if (board[y][i] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	return valid;
}

void highlightValid(int pieceToMove){
  for (int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      //Serial.println(validateMove(pieceToMove, i, j));
      if ( validateMove(pieceToMove,i,j) && checkObstruction(pieceToMove,i,j) ){
        highlightSquare(i,j,0x600F);
      }
    }
  }
}

void unhighlightValid(int pieceToMove){
  for (int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      //Serial.println(validateMove(pieceToMove, i, j));
      if ( validateMove(pieceToMove,i,j) && checkObstruction(pieceToMove,i,j) ){
        unhighlightSquare(i,j);
      }
    }
  }
}
