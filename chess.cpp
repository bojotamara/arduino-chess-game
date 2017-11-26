/*
	Chess game final project
*/

#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include "lcd_image.h"

#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define BOARD_SIZE 240

#define JOY_VERT  A1 // should connect A1 to pin VRx
#define JOY_HORIZ A0 // should connect A0 to pin VRy
#define JOY_SEL   2

#define JOY_CENTER   512
#define JOY_DEADZONE 64

// common colors
#define WHITE 0xFFFF
uint16_t BROWN = tft.color565(139,69,19);
uint16_t BEIGE = tft.color565(205,133,63);

//#TODO: add these files to the sd card
// images of the pieces
lcd_image_t whitePawnImg = {"whitepawn.lcd", 30 , 30};
lcd_image_t whiteRookImg = {"whiterook.lcd", 30 , 30};
lcd_image_t whiteBishopImg = {"whitebishop.lcd", 30 , 30};
lcd_image_t whiteKnightImg = {"whiteknight.lcd", 30 , 30};
lcd_image_t whiteKingImg = {"whiteking.lcd", 30 , 30};
lcd_image_t whiteQueenImg = {"whitequeen.lcd", 30 , 30};

lcd_image_t blackPawnImg = {"blackpawn.lcd", 30 , 30};
lcd_image_t blackRookImg = {"blackrook.lcd", 30 , 30};
lcd_image_t blackBishopImg = {"blackbishop.lcd", 30 , 30};
lcd_image_t blackKnightImg = {"blackknight.lcd", 30 , 30};
lcd_image_t blackKingImg = {"blackking.lcd", 30 , 30};
lcd_image_t blackQueenImg = {"blackqueen.lcd", 30 , 30};

lcd_image_t yegImage = { "yeg-big.lcd", 2048, 2048 };

// hold the position of square selected
int selectedY = 0;
int oldSelectedY;
int selectedX = 0;
int oldSelectedX;

//2-D array that represents the board
int board [8][8];

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
				sqColor = BROWN; //black
			}
			else {
				sqColor = BEIGE; // yellow
			}
			//offset the row by 1
			tft.fillRect( (BOARD_SIZE/8)*i , BOARD_SIZE/8 + (BOARD_SIZE/4)*j , BOARD_SIZE/8, BOARD_SIZE/8, sqColor);
		}
	}


}

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


void drawPiece() {

}

/*
Function that clears a square (sets it back to its original color)
@param: The x and y index of the square you want to highlight
				So emptySquare(0,0) will empty the top left square
*/
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

void colorSquare(int squarex, int squarey) {
	tft.fillRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, WHITE );
}

/* Highlights a square
@param: The x and y index of the square you want to highlight
				So emptySquare(0,0) will highlight the top left square
*/
void highlightSquare(int squarex, int squarey) {
	tft.drawRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, 0xFFE0 );
}

/*
Function that allows the player to scroll through the squares to highlight them
*/
void scroll() {
	oldSelectedY = selectedY;
	oldSelectedX = selectedX;

	int yVal = analogRead(JOY_VERT);
	int xVal = analogRead(JOY_HORIZ);

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
		highlightSquare(selectedX,selectedY);
		emptySquare(oldSelectedX,oldSelectedY);
	}

	delay(100);

}


void setup() {
	init();
	Serial.begin(9000);
	tft.begin();
	pinMode(JOY_SEL, INPUT_PULLUP);

	tft.setRotation(3);

	drawBoard();
	fillBoardArray();
}

int main() {
	setup();

	while(true) {

		scroll();

		// when joystick is pressed
		if (digitalRead(2) == 0) {
			colorSquare(selectedX,selectedY);
			highlightSquare(selectedX,selectedY);
		}

	}

	Serial.end();
	return 0;
}
