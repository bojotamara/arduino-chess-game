/*
	Chess game final project
*/
#include <Arduino.h>
#include "chessfunctions.h"

#define JOY1_SEL   2
#define JOY2_SEL   8

//keeps track of current player
//1 is player 1, and 2 is player 2
int currentplayer=1;

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
					dispCurrentPlayer(); //updates sidemenu
				}
				break;

			case 2:
				if (digitalRead(JOY2_SEL) == 0) {
					while (digitalRead(JOY2_SEL) == LOW) { delay(10); }
					delay(100);
					moveMode();
					dispCurrentPlayer();
				}
				break;

		}

	}

	Serial.end();
	return 0;
}
