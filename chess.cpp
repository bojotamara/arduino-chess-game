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





#define JOY1_VERT  A1 // should connect A1 to pin VRx
#define JOY1_HORIZ A0 // should connect A0 to pin VRy
#define JOY1_SEL   2

#define JOY2_VERT  A5 //player 2 VRx
#define JOY2_HORIZ A4 //         VRy
#define JOY2_SEL   8

// #define JOY_CENTER   512
// #define JOY_DEADZONE 64

//keeps track of current player
//1 is player 1, and 2 is player 2
int currentplayer;








int main() {
	setup();

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
