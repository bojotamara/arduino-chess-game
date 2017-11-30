/*
	Chess game final project
*/

#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
//#include "lcd_image.h"
#include "chessfunctions.h"


#define TFT_DC 9
#define TFT_CS 10
#define SD_CS 6

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define DISPLAY_WIDTH  320
#define DISPLAY_HEIGHT 240
#define BOARD_SIZE 240

#define JOY1_VERT  A1 // should connect A1 to pin VRx
#define JOY1_HORIZ A0 // should connect A0 to pin VRy
#define JOY1_SEL   2

#define JOY2_VERT  A5 //player 2 VRx
#define JOY2_HORIZ A4 //         VRy
#define JOY2_SEL   8

#define JOY_CENTER   512
#define JOY_DEADZONE 64

//keeps track of current player
//1 is player 1, and 2 is player 2
int currentplayer=1;

// hold the position of square selected
int selectedY = 0;
int oldSelectedY;
int selectedX = 0;
int oldSelectedX;

//2-D array that represents the board
int board [8][8];




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
	tft.println("hi");
}

int main() {
	setup();
	tft.setCursor(DISPLAY_WIDTH-(DISPLAY_WIDTH- BOARD_SIZE),0);

	while(true) {

		scroll();


		switch (currentplayer){
			case 1:
			// when joystick is pressed
				if (digitalRead(JOY1_SEL) == 0) {
					while (digitalRead(JOY1_SEL) == LOW) { delay(10); }
					delay(100);
					moveMode();
				}
				break;

			case 2:
				if (digitalRead(JOY2_SEL) == 0) {
					while (digitalRead(JOY2_SEL) == LOW) { delay(10); }
					delay(100);
					moveMode();
				}
				break;

		}



	}

	Serial.end();
	return 0;
}
