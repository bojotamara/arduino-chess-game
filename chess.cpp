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

// hold the position of square selected
int selectedY = 0;
int oldSelectedY;
int selectedX = 0;
int oldSelectedX;


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

void selectSquare(int squarex, int squarey) {
	tft.drawRect( (BOARD_SIZE/8)*squarex , (BOARD_SIZE/8)*squarey , BOARD_SIZE/8, BOARD_SIZE/8, 0xFFE0 );
}

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
		selectSquare(selectedX,selectedY);
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
}



int main() {
	setup();
	// int oldSelectedY;
	// int selectedY = 0;

	while(true) {

		scroll();

		// when joystick is pressed
		if (digitalRead(2) == 0) {
			colorSquare(selectedX,selectedY);
			selectSquare(selectedX,selectedY);
		}

	}

	return 0;
}
