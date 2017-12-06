#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include "lcd_image.h"

#include "chessfunctions.h"
#include "validmoves.h"
#include "specialcases.h"
/*
================================================================================
LCD STUFF
*/
#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6
#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// common colors
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define BLACK 0x0000
#define CYAN 0x07FF
uint16_t BROWN = tft.color565(139,69,19);
uint16_t BEIGE = tft.color565(205,133,63);
#define CHOCOBROWN 0x8AE7
#define GREEN 0x07E0


// images of the pieces
// dark and light refers to the background color
lcd_image_t wPawnImgDARK = {"icons/wpawnD.lcd", 25 , 25};
lcd_image_t wRookImgDARK = {"icons/wrookD.lcd", 25 , 25};
lcd_image_t wBishopImgDARK = {"icons/wbishopD.lcd", 25 , 25};
lcd_image_t wKnightImgDARK = {"icons/wknightD.lcd", 25 , 25};
lcd_image_t wKingImgDARK = {"icons/wkingD.lcd", 25 , 25};
lcd_image_t wQueenImgDARK = {"icons/wqueenD.lcd", 25 , 25};
lcd_image_t bPawnImgDARK = {"icons/bpawnD.lcd", 25 , 25};
lcd_image_t bRookImgDARK = {"icons/brookD.lcd", 25 , 25};
lcd_image_t bBishopImgDARK = {"icons/bbishopD.lcd", 25 , 25};
lcd_image_t bKnightImgDARK = {"icons/bknightD.lcd", 25 , 25};
lcd_image_t bKingImgDARK = {"icons/bkingD.lcd", 25 , 25};
lcd_image_t bQueenImgDARK = {"icons/bqueenD.lcd", 25 , 25};

lcd_image_t wPawnImgLIGHT = {"icons/wpawnL.lcd", 25 , 25};
lcd_image_t wRookImgLIGHT = {"icons/wrookL.lcd", 25 , 25};
lcd_image_t wBishopImgLIGHT = {"icons/wbishopL.lcd", 25 , 25};
lcd_image_t wKnightImgLIGHT = {"icons/wknightL.lcd", 25 , 25};
lcd_image_t wKingImgLIGHT = {"icons/wkingL.lcd", 25 , 25};
lcd_image_t wQueenImgLIGHT = {"icons/wqueenL.lcd", 25 , 25};
lcd_image_t bPawnImgLIGHT = {"icons/bpawnL.lcd", 25 , 25};
lcd_image_t bRookImgLIGHT = {"icons/brookL.lcd", 25 , 25};
lcd_image_t bBishopImgLIGHT = {"icons/bbishopL.lcd", 25 , 25};
lcd_image_t bKnightImgLIGHT = {"icons/bknightL.lcd", 25 , 25};
lcd_image_t bKingImgLIGHT = {"icons/bkingL.lcd", 25 , 25};
lcd_image_t bQueenImgLIGHT = {"icons/bqueenL.lcd", 25 , 25};
/*
================================================================================
*/


/*
================================================================================
JOYSTICK STUFF
*/
#define JOY1_VERT  A1 // should connect A1 to pin VRx
#define JOY1_HORIZ A0 // should connect A0 to pin VRy
#define JOY1_SEL   2

#define JOY2_VERT  A5 //player 2 VRx
#define JOY2_HORIZ A4 //         VRy
#define JOY2_SEL   8

#define JOY_CENTER   512
#define JOY_DEADZONE 64
/*
================================================================================
*/

/*
================================================================================
MATRIX STUFF FOR CHESSBOARD
*/

// integers to represent types of pieces
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

//2-D array that represents the board
int board [8][8];

#define BOARD_SIZE 240

// hold the position of square selected
int selectedY = 0;
int oldSelectedY;
int selectedX = 0;
int oldSelectedX;
int chosenX=10;
int chosenY=10;

//castling
bool p1_kingMoved=0;
bool p1_leftRookmoved=0;
bool p1_rightRookmoved=0;
bool p2_kingMoved=0;
bool p2_leftRookmoved=0;
bool p2_rightRookmoved=0;
/*
================================================================================
/*


/*
DRAWING FUNCTIONS
================================================================================
*/

/*
Function that draws an empty board to the screen
*/
void drawBoard() {
	uint16_t sqColor;
	//clear the screen first
	tft.fillScreen(0);
	//rows where beige is first, brown is second
	for (int j=0; j < 4; j++) {
		for (int i=0; i < 8; i++) {
			if (i%2 == 0) {
				sqColor = BEIGE; //yellow
			}
			else {
				sqColor = BROWN; // black
			}
			tft.fillRect( (BOARD_SIZE/8)*i , (BOARD_SIZE/4)*j , BOARD_SIZE/8, BOARD_SIZE/8, sqColor);
		}
	}

	//rows where brown is first
	for (int j=0; j < 4; j++) {
		for (int i=0; i < 8; i++) {
			if (i%2 == 0) {
				sqColor = BROWN;
			}
			else {
				sqColor = BEIGE;
			}
			//offset the row by 1
			tft.fillRect( (BOARD_SIZE/8)*i , BOARD_SIZE/8 + (BOARD_SIZE/4)*j , BOARD_SIZE/8, BOARD_SIZE/8, sqColor);
		}
	}

	// fills in right menu
	tft.fillRect(BOARD_SIZE,0,DISPLAY_WIDTH-BOARD_SIZE,DISPLAY_HEIGHT,BLACK);

}

/*
Function that draws the contents of the board array to the screen
*/
void drawArray() {
	int piece;

	for (int i=0; i<8; i++) {
		for (int j=0;j<8;j++) {

			piece = board[i][j];

			drawPiece(j,i,piece);

		}
	}
}


void emptySquare(int squarex, int squarey) {
	int sqColor;
	if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
		sqColor = BROWN;
	}
	else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
		sqColor = BEIGE;
	}
	tft.fillRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, sqColor);
}

/*
Function that draws a specified piece onto a specified squarex and squarey,
also uploading a different backgroud color based on the square
*/
void drawPiece(int squarex, int squarey, int piecetype) {

	lcd_image_t image;
	bool isEmpty = false;

	switch (piecetype) {
		case EMPTY:
			isEmpty = true;
			break;
		case W_ROOK:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = wRookImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = wRookImgLIGHT;
			}
			break;
		case W_KNIGHT:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = wKnightImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = wKnightImgLIGHT;
			}
			break;
		case W_BISHOP:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = wBishopImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = wBishopImgLIGHT;
			}
			break;
		case W_KING:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = wKingImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = wKingImgLIGHT;
			}
			break;
		case W_QUEEN:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = wQueenImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = wQueenImgLIGHT;
			}
			break;
		case W_PAWN:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = wPawnImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = wPawnImgLIGHT;
			}
			break;
		case B_ROOK:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = bRookImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = bRookImgLIGHT;
			}
			break;
		case B_KNIGHT:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = bKnightImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = bKnightImgLIGHT;
			}
			break;
		case B_BISHOP:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = bBishopImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = bBishopImgLIGHT;
			}
			break;
		case B_KING:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = bKingImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = bKingImgLIGHT;
			}
			break;
		case B_QUEEN:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = bQueenImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = bQueenImgLIGHT;
			}
				break;
		case B_PAWN:
			if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
				image = bPawnImgDARK;
			}
			else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
				image = bPawnImgLIGHT;
			}
		break;
	}

	if (isEmpty == false) {
		lcd_image_draw(&image, &tft,1,1,(BOARD_SIZE/8)*squarex +3,(BOARD_SIZE/8)*squarey+3,24, 24);
	}

	else {
		emptySquare(squarex,squarey);
	}

}


/*
Function that clears a square (sets it back to its original state)
@param: The x and y index of the square you want to revert
				So unhighlightSquare(0,0) will empty the top left square
*/
void unhighlightSquare(int squarex, int squarey) {
	int sqColor;
	if ( (squarex % 2 !=0 && squarey % 2 == 0) || (squarex % 2 ==0 && squarey % 2 != 0) ) {
		sqColor = BROWN;
	}
	else if ( (squarex % 2 ==0 && squarey % 2 == 0) || (squarex % 2 !=0 && squarey % 2 != 0) ) {
		sqColor = BEIGE;
	}
	tft.drawRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, sqColor);
	//tft.fillRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, sqColor);
}

/* Highlights a square
@param: The x and y index of the square you want to highlight
				So unhighlightSquare(0,0) will highlight the top left square
*/
void highlightSquare(int squarex, int squarey, uint16_t bordercolor) {
	if (bordercolor == RED) {
		tft.drawRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, RED);
	}
	else if (bordercolor == GREEN) {
		tft.drawRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, GREEN);
	}
	else if (bordercolor == 0x600F){
		tft.drawRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, bordercolor );
	}
	else if (currentplayer == 1 ) {
		tft.drawRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, YELLOW);
	}
	else if (currentplayer == 2) {
		tft.drawRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, CYAN );
	}

}

void dispCurrentPlayer(){
	tft.setCursor(BOARD_SIZE+8,8);
	tft.setTextSize(6);

	switch (currentplayer){
		case 1:
			tft.setTextColor(YELLOW,BLACK);
			tft.print("P1");
			break;

		case 2:
			tft.setTextColor(CYAN,BLACK);
			tft.print("P2");
			break;
	}
}

void dispTips(String tip){
	//keep this I'm using it for checking boundaries when debugging
	//tft.fillRect(BOARD_SIZE,60,DISPLAY_WIDTH-BOARD_SIZE,80,BROWN);
	tft.fillRect(BOARD_SIZE,60,DISPLAY_WIDTH-BOARD_SIZE,120,BLACK);
	tft.setCursor(BOARD_SIZE+5,60);
	tft.setTextSize(1);

	if (tip =="wrongpiece" || tip == "emptysquare" || tip == "invalidmove" || tip == "leavecheck"){
		highlightSquare(selectedX,selectedY,RED);
		if(tip == "wrongpiece"){
			tft.println("Oops! You");
			tft.setCursor(BOARD_SIZE+5,70);
			tft.println("picked your");
			tft.setCursor(BOARD_SIZE+5,80);
			tft.println("opponent's");
			tft.setCursor(BOARD_SIZE+5,90);
			tft.println("piece, try");
			tft.setCursor(BOARD_SIZE+5,100);
			tft.println("picking one");
			tft.setCursor(BOARD_SIZE+5,110);
			tft.println("of yours!");
		}
		else if(tip == "emptysquare"){
			tft.println("You didn't");
			tft.setCursor(BOARD_SIZE+5,70);
			tft.println("select a");
			tft.setCursor(BOARD_SIZE+5,80);
			tft.println("piece to");
			tft.setCursor(BOARD_SIZE+5,90);
			tft.println("move!");
		}
		else if(tip == "invalidmove"){
			tft.println("Oops, you");
			tft.setCursor(BOARD_SIZE+5,70);
			tft.println("chose an");
			tft.setCursor(BOARD_SIZE+5,80);
			tft.println("invalid spot");
			tft.setCursor(BOARD_SIZE+5,90);
			tft.println("to move to");

		}
		else if(tip == "leavecheck"){
			tft.println("Oops, you");
			tft.setCursor(BOARD_SIZE+5,70);
			tft.println("must get");
			tft.setCursor(BOARD_SIZE+5,80);
			tft.println("out of the");
			tft.setCursor(BOARD_SIZE+5,90);
			tft.println("check!");

		}
		delay(2000);

		highlightSquare(selectedX,selectedY, YELLOW);

	}
	else if(tip=="select"){
		tft.println("Pick a piece");
		tft.setCursor(BOARD_SIZE+5,70);
		tft.println("to move");
	}

	else if(tip == "move"){
		tft.println("Choose a");
		tft.setCursor(BOARD_SIZE+5,70);
		tft.println("spot to move");
		tft.setCursor(BOARD_SIZE+5,80);
		tft.println("it to. Or");
		tft.setCursor(BOARD_SIZE+5,90);
		tft.println("cancel by");
		tft.setCursor(BOARD_SIZE+5,100);
		tft.println("clicking on");
		tft.setCursor(BOARD_SIZE+5,110);
		tft.println("the chosen");
		tft.setCursor(BOARD_SIZE+5,120);
		tft.println("piece again");
		tft.setCursor(BOARD_SIZE+5,130);
		tft.println("then pick");
		tft.setCursor(BOARD_SIZE+5,140);
		tft.println("a different");
		tft.setCursor(BOARD_SIZE+5,150);
		tft.println("piece");
	}


	else if(tip == "promotion"){
		tft.println("Your pawn");
		tft.setCursor(BOARD_SIZE+5,70);
		tft.println("has been");
		tft.setCursor(BOARD_SIZE+5,80);
		tft.println("promoted to");
		tft.setCursor(BOARD_SIZE+5,90);
		tft.println("a queen!");

	}

	else if(tip == "castled"){
		tft.println("You have");
		tft.setCursor(BOARD_SIZE+5,70);
		tft.println("castled!");
	}

  else if (tip == "check") {
    tft.setTextSize(2);
    tft.setTextColor(RED,BLACK);

    tft.setCursor(BOARD_SIZE+7,200);
    tft.println("Check!");
   }
}

/*
GAME LOGIC
================================================================================
*/

/*
Function that fills the board array with the appropriate pieces at the start
of a chess game
*/
void fillBoardArray() {
	//pawns
	for (int i=0; i<8; i++) {
		board[6][i] = W_PAWN;
		board[1][i] = B_PAWN;
	}

	// bottom/ white pieces
	board[7][0] = W_ROOK;
	board[7][1] = W_KNIGHT;
	board[7][2] = W_BISHOP;
	board[7][3] = W_QUEEN;
	board[7][4] = W_KING;
	board[7][5] = W_BISHOP;
	board[7][6] = W_KNIGHT;
	board[7][7] = W_ROOK;

	// top/ black pieces
	board[0][0] = B_ROOK;
	board[0][1] = B_KNIGHT;
	board[0][2] = B_BISHOP;
	board[0][3] = B_QUEEN;
	board[0][4] = B_KING;
	board[0][5] = B_BISHOP;
	board[0][6] = B_KNIGHT;
	board[0][7] = B_ROOK;

	//empty spaces
	for (int i=2; i < 6; i++) {
		for (int j=0; j<8; j++) {
			board[i][j] = EMPTY;
		}
	}

}

/*
Function that allows the player to scroll through the squares to highlight them
*/
void scroll() {
	oldSelectedY = selectedY;
	oldSelectedX = selectedX;
	int xVal, yVal;


	switch (currentplayer){
		case 1:
			yVal = analogRead(JOY1_VERT);
			xVal = analogRead(JOY1_HORIZ);
			break;

		case 2:
			yVal = analogRead(JOY2_VERT);
			xVal = analogRead(JOY2_HORIZ);
			break;
	}


	if (yVal < JOY_CENTER - JOY_DEADZONE) {
		selectedY -= 1;
	}

	else if (yVal > JOY_CENTER + JOY_DEADZONE) {
		selectedY += 1;
	}

	if (xVal < JOY_CENTER - JOY_DEADZONE) {
		selectedX += 1;
	}

	else if (xVal > JOY_CENTER + JOY_DEADZONE) {
		selectedX -= 1;
	}

	selectedY = constrain(selectedY,0,7);
	selectedX = constrain(selectedX,0,7);

	if (oldSelectedY != selectedY || oldSelectedX != selectedX) {
		if (selectedX == chosenX && selectedY == chosenY) {
			//
		}
		else {
			highlightSquare(selectedX,selectedY);
		}
		if (oldSelectedX == chosenX && oldSelectedY == chosenY) {
			//
		} else {
			unhighlightSquare(oldSelectedX,oldSelectedY);
		}


	}

	delay(100);


}

/*
Function that moves a piece to the selected square, and updates the board array
*/
void movePiece(int oldx, int oldy, int pieceToMove) {

	board[oldy][oldx] = EMPTY;
	board[selectedY][selectedX] = pieceToMove;
	emptySquare(oldx,oldy);
	drawPiece(selectedX,selectedY,pieceToMove);
}

void moveMode() {
	bool checkBlack;
	bool checkWhite;

	int pieceToMove = board[selectedY][selectedX];
	if (pieceToMove == EMPTY) {
		//tft.println("Can't move empty square");
		dispTips("emptysquare");
		return;
	}
	else if (pieceToMove > 0 && currentplayer == 2) {
		//cant move opponents piece

		//flash square red to let player know that square can't be moved
		//highlightSquare(selectedX,selectedY,RED);
		dispTips("wrongpiece");
		//unhighlightSquare(selectedX,selectedY);
		return;
	}
	else if (pieceToMove < 0 && currentplayer == 1) {
		// cant move opponents piece

		//flash square red to let player know that square can't be moved
		//highlightSquare(selectedX,selectedY,RED);
		dispTips("wrongpiece");
		//unhighlightSquare(selectedX,selectedY);
		return;
	}
	else{
		dispTips("move");
	}



	chosenX = selectedX;
	chosenY = selectedY;

	highlightSquare(chosenX,chosenY,GREEN);
	//green for valid square chosen

	//show valid moves
	highlightValid(pieceToMove);

	while(true) {
		scroll();

		if (currentplayer == 1 && digitalRead(JOY1_SEL)==0){
			while (digitalRead(JOY1_SEL) == LOW) { delay(10); }

			if (selectedX == chosenX && selectedY == chosenY){
				//same piece selected, cancel the move
				break;
			}

			checkWhite = checkOnWhite(pieceToMove,selectedX,selectedY);

			bool valid = validateMove(pieceToMove,selectedX,selectedY,board) && checkObstruction(pieceToMove,selectedX,selectedY,board);

			bool specialcase = checkSpecialcases(selectedX,selectedY,pieceToMove);
			
			if (specialcase && !checkWhite){
				//if a special case already happened, that function took care of
				//moving the pieces already

				//clear the check message
				tft.fillRect(BOARD_SIZE,180,DISPLAY_WIDTH-BOARD_SIZE,240-180,BLACK);
				currentplayer=2;
				break;
			}


			else if (valid && !checkWhite) {
				//move is valid so move the piece
				movePiece(chosenX,chosenY,pieceToMove);
				currentplayer = 2;

				//clear the check message
				tft.fillRect(BOARD_SIZE,180,DISPLAY_WIDTH-BOARD_SIZE,240-180,BLACK);

				//end the move
				break;
			}

			else if(!valid && !checkWhite){
				dispTips("invalidmove");
				dispTips("move");
			}
			else if (checkWhite) {
				dispTips("leavecheck");
				dispTips("move");
			}
		}


		else if (currentplayer ==2 && digitalRead(JOY2_SEL)==0){
			while (digitalRead(JOY2_SEL) == LOW) { delay(10); }

			if (selectedX == chosenX && selectedY == chosenY){
				//same piece selected, cancel the move
				break;
			}

			checkBlack = checkOnBlack(pieceToMove,selectedX,selectedY);

			bool valid = validateMove(pieceToMove,selectedX,selectedY,board) && checkObstruction(pieceToMove,selectedX,selectedY,board);

			bool specialcase = checkSpecialcases(selectedX,selectedY,pieceToMove);

			if (specialcase && !checkBlack){
				//if a special case already happened, that function took care of
				//moving the pieces already

				//clear the check message
				tft.fillRect(BOARD_SIZE,180,DISPLAY_WIDTH-BOARD_SIZE,240-180,BLACK);
				currentplayer=1;
				break;
			}

			else if (valid && !checkBlack) {
				movePiece(chosenX,chosenY,pieceToMove);
				currentplayer = 1;

				//clear the check message
				tft.fillRect(BOARD_SIZE,180,DISPLAY_WIDTH-BOARD_SIZE,240-180,BLACK);
				//end the move
				break;
			}
			else if(!valid && !checkBlack){
				dispTips("invalidmove");
				dispTips("move");
			}
			else if (checkBlack) {
				dispTips("leavecheck");
				dispTips("move");
			}

		}


	}

	unhighlightValid(pieceToMove);
	// set chosen piece to nothing
	highlightSquare(selectedX,selectedY);
	chosenX = 10;
	chosenY = 10;




	if (checkOnWhite()) {
		if (checkmate("white")) {
			gameover = 2;
		}
		else {
			dispTips("check");
		}

	}
	else if (checkOnBlack()) {
		if (checkmate("black")) {
			gameover = 1;


		}
		else {
			dispTips("check");
		}

	}



}

void endGame( int player) {
	unhighlightSquare(selectedX,selectedY);
	tft.fillRect(BOARD_SIZE,0,DISPLAY_WIDTH-BOARD_SIZE,DISPLAY_HEIGHT,BLACK);
	tft.setTextSize(2);
	tft.setCursor(BOARD_SIZE+10,10);
	tft.setTextColor(RED,BLACK);
	tft.println("CHECK");
	tft.setCursor(BOARD_SIZE+10,30);
	tft.println("MATE!");
	tft.setTextSize(3);
	tft.setCursor(BOARD_SIZE+22,70);

	switch (player) {

		case 1:
			tft.setTextColor(YELLOW,BLACK);
			tft.println("P1");
			break;
		case 2:
			tft.setTextColor(CYAN,BLACK);
			tft.println("P2");
			break;
	}

	tft.setTextSize(2);
	tft.setCursor(BOARD_SIZE+13,100);
	tft.println("WINS!");

}
/*
================================================================================
*/

void setup() {
	init();
	Serial.begin(9600);
	tft.begin();
	pinMode(JOY1_SEL, INPUT_PULLUP);
	pinMode(JOY2_SEL,INPUT_PULLUP);

	Serial.print("Initializing SD card...");
	if (!SD.begin(SD_CS)) {
			Serial.println("failed! Is it inserted properly?");
			while (true) {}
		}
		Serial.println("OK!");

	tft.setRotation(3);

	drawBoard();
	fillBoardArray();
	drawArray();
	tft.setCursor(DISPLAY_WIDTH-(DISPLAY_WIDTH- BOARD_SIZE),0);
	dispCurrentPlayer();
	dispTips("select");
}
