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
bool validateMove(int piecetomove, int selX, int selY, int boardtouse[][8]) {
	bool valid = false;

	switch (piecetomove) {
		//need to work on pawns moving 2 spaces on first turn, and leater en passant
		case W_PAWN:
			// valid if pawn is moved forward on an empty space
			if (boardtouse[selY][selX] == EMPTY && selX == chosenX){
				//normal move forward
				if (selY == chosenY -1) {
					valid = true;
				}
				//2 spaces at the beginning
				else if (selY == chosenY - 2 && chosenY == 6 && boardtouse[selY+1][selX] == EMPTY) {
					valid = true;
				}
			}
			//valid if diagonal, and pawn eats opponent
			else if ((selX == chosenX -1 || selX == chosenX + 1) && selY == chosenY - 1 && boardtouse[selY][selX] < 0 ) {
				valid = true;
			}
			break;

		case B_PAWN:
			// valid if pawn is moved forward on an empty space
			if (boardtouse[selY][selX] == EMPTY && selX == chosenX){
				//normal move forward
				if (selY == chosenY + 1) {
					valid = true;
				}
				//2 spaces at the beginning
				else if (selY == chosenY + 2 && chosenY == 1 && boardtouse[selY-1][selX] == EMPTY) {
					valid = true;
				}
			}
			//valid if diagonal, and pawn eats opponent
			else if ((selX == chosenX -1 || selX == chosenX + 1) && selY == chosenY + 1 && boardtouse[selY][selX] > 0 ) {
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
			//diagonals, so (x,y) = (x+/-i,y+/-i) or (x+/-i,y-/+i)
			for(int i=-7; i<8;i++){

				if(selX==chosenX+i && selY==chosenY+i || selX==chosenX-i && selY == chosenY+i){
					valid=true;
					break;
				}
			}
			break;


		case B_BISHOP:

			for(int i=-7; i<8;i++){
				
				if(selX==chosenX+i && selY==chosenY+i || selX==chosenX-i && selY == chosenY+i){
					valid=true;
					break;
				}
			}
			break;


		case W_KING:
			//valid for 2 blocks up/down and 1 to the right/left
			if(abs(selX-chosenX)<=1 && abs(selY-chosenY)<=1 && boardtouse[selY][selX] <= 0){
				valid=true;
			}
			break;

		case B_KING:
			//valid for 2 blocks up/down and 1 to the right/left
			if(abs(selX-chosenX)<=1 && abs(selY-chosenY)<=1 && boardtouse[selY][selX] >=0){
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
	if (currentplayer == 1 && boardtouse[selY][selX] > 0) {
		valid = false;
	}
	else if ( currentplayer == 2 && boardtouse[selY][selX] < 0) {
		valid = false;
	}

	return valid;
}

bool checkObstruction(int piece, int selX, int selY, int boardtouse[][8]) {
	bool valid = true;

	// checking the obstruction for vertical, upward lines of attack
	if ( (abs(piece) == W_QUEEN || abs(piece) == W_ROOK) && selX == chosenX && selY < chosenY) {
		for (int i = chosenY-1; i > selY; i--) {
			if (boardtouse[i][chosenX] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// checking the obstruction for vertical, downward lines of attack
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_ROOK) && selX == chosenX && selY > chosenY) {
		for (int i = chosenY+1; i < selY; i++) {
			if (boardtouse[i][chosenX] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// horizontal, left lines of attack
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_ROOK) && selY == chosenY && selX < chosenX) {
		for (int i = chosenX-1; i > selX; i--) {
			if (boardtouse[chosenY][i] != EMPTY) {
				valid = false;
				break;
			}
		}
	}

	// horizontal, right lines of attack
	else if ( (abs(piece) == W_QUEEN || abs(piece) == W_ROOK) && selY == chosenY && selX > chosenX) {
		for (int i = chosenX+1; i < selX; i++) {
			if (boardtouse[chosenY][i] != EMPTY) {
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
			if (boardtouse[y][i] != EMPTY) {
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
			if (boardtouse[y][i] != EMPTY) {
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
			if (boardtouse[y][i] != EMPTY) {
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
			if (boardtouse[y][i] != EMPTY) {
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
      if ( validateMove(pieceToMove,i,j,board) && checkObstruction(pieceToMove,i,j,board) ){
        highlightSquare(i,j,0x600F);
      }
    }
  }
}

void unhighlightValid(int pieceToMove){
  for (int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      //Serial.println(validateMove(pieceToMove, i, j));
      if ( validateMove(pieceToMove,i,j,board) && checkObstruction(pieceToMove,i,j,board) ){
        unhighlightSquare(i,j);
      }
    }
  }
}

bool checkOnBlack(int piece) {
	bool check = false;
	int actualChosenX = chosenX;
	int actualChosenY = chosenY;
	int actualplayer = currentplayer;

	int fakeboard[8][8];
	memcpy(fakeboard, board, 8*8*sizeof(int));
	fakeboard[chosenY][chosenX] = EMPTY;
	fakeboard[selectedY][selectedX] = piece;

	//in order for the checks to work, we have to 'fake' the current player
	//and the chosen for the appropriate situation
	currentplayer = 1;
	//iterate through every square to find pieces
	for (int i=0; i<8; i++){
    for(int j=0; j<8; j++){
			//find the white pieces
			if (fakeboard[j][i] > 0) {

				//interate through every square for every white piece to see if they're targetting the black king
				for (int a=0; a<8; a++){
			    for(int b=0; b<8; b++){
						chosenX = i;
						chosenY = j;

						//if move is valid and targetting the king
			      if ( fakeboard[b][a] == B_KING && validateMove(fakeboard[j][i],a,b,fakeboard) && checkObstruction(fakeboard[j][i],a,b,fakeboard) ){
							check = true;
			      }



			    }
			  }
			}

    }
  }

	//revert the changes necessary for the checks
	currentplayer = actualplayer;
	chosenX = actualChosenX;
	chosenY = actualChosenY;
	return check;

}

bool checkOnWhite(int piece) {
	bool check = false;

	int actualChosenX = chosenX;
	int actualChosenY = chosenY;
	int actualplayer = currentplayer;

	//in order to make the potential move for the check, but not affect the actual board
	int fakeboard[8][8];
	memcpy(fakeboard, board, 8*8*sizeof(int));
	fakeboard[chosenY][chosenX] = EMPTY;
	fakeboard[selectedY][selectedX] = piece;

	//in order for the checks to work, we have to 'fake' the current player
	//and the chosen for the appropriate situation
	currentplayer = 2;
	//iterate through every square to find pieces
	for (int i=0; i<8; i++){
    for(int j=0; j<8; j++){
			//find the black pieces
			if (fakeboard[j][i] < 0) {

				//interate through every square for every white piece to see if they're targetting the white king
				for (int a=0; a<8; a++){
			    for(int b=0; b<8; b++){
						chosenX = i;
						chosenY = j;

						//if move is valid and targetting the king
			      if (validateMove(fakeboard[j][i],a,b,fakeboard) && checkObstruction(fakeboard[j][i],a,b,fakeboard) && fakeboard[b][a] == W_KING ){
							check = true;
			      }

			    }
			  }
			}

    }
  }

	//revert the changes necessary for the checks
	currentplayer = actualplayer;
	chosenX = actualChosenX;
	chosenY = actualChosenY;
	return check;

}
