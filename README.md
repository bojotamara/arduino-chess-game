###### Arun Woosaree
###### Tamara Bojovic


# CMPUT 274 Final Project - Chess Game
--------------------------------------------------------------------------------
For our final project, we decided to make a player versus player implementation
of chess on a single Arduino mega board. It includes all the essential rules
for chess. A graphical representation of the board is displayed on the Arduino
with a side menu with contextual tips, that displays useful information such as
whose turn it is, if the move you're attempting is invalid, if someone is in
check, among other things. There are two joysticks, one for each player.
On your turn, you can use your joystick to select pieces and then move them.
The program only allows you to make valid moves according to the rules of chess,
and also includes the special moves: castling, en passant, and promotion.
The program automatically detects check and checkmate, and for the latter,
ends the game and declares the winner.

For more specific instructions on how to play, see 'Usage Instructions'.

## Accessories:
--------------------------------------------------------------------------------
-1 x Arduino Mega Board
-2 x SainSmart Joysticks
-1 x Adafruit ILI9341 display
-1 x 8GB SD Card

** Alternatively, if you only have access to one joystick. You can move it back
    and forth between the location of the wiring, to play.
** REQUIRED SD card files are included in the folder, labelled 'icons'.
    Copy this folder to the root of the SD card, and put it in the SD card reader
    on the display.

## Wiring Instructions:
--------------------------------------------------------------------------------

### Player 1:
Joystick |  Arduino
---------|------------------
GND  |  GND (board bus)
5V   |  5V (board bus)
VRX  |  Analog Pin 1
VRY  |  Analog Pin 0
SW   |  Digital Pin 2


### Player 2:
Joystick  | Arduino
-----|----------------------
GND  |  GND (board bus)
5V   |  5V (board bus)
VRX  |  Analog Pin 5
VRY  |  Analog Pin 4
SW   |  Digital Pin 8

### Display
Display    |          Arduino
-----------|------------------------
GND        |       GND
Vin        |       5V
3Vo        |       NOT CONNECTED
CLK        |       52
MISO       |       50
MOSI       |       51
CS         |       10
D/C        |       9
RST        |       NOT CONNECTED
Lite       |       NOT CONNECTED
Y+         |       A2 (analog pin)
X+         |       4  (digital pin)
Y-         |       5  (digital pin)
X-         |       A3 (analog pin)
IM0 - IM3  |       NOT CONNECTED (they expect a 3.3v signal, DON'T CONNECT!)
CCS        |       6
CD         |       NOT CONNECTED

![Wiring](/fritzing_files/arduinochess_bb.png)
## Usage Instructions:
--------------------------------------------------------------------------------
Copy the 'icons' folder onto the SD card. Without this, pictures of the pieces
can't be accessed and therefore the game can't be played. Right click the 'icons'
folder and select copy, and then with the SD card inserted into your computer,
paste it onto the SD card. Insert the SD card into the Arduino. Connect the
Arduino to the computer. Open a terminal window in the same directory as the
project folder. Then, do a

```
make upload
```


You should see a chess game load, and then of course, P1 can immediately start
his/her turn. Once Player 1 completes their turn, control will be passed to
Player 2. The players take turns, until one player wins by putting the other in checkmate.

#### Moving A Piece On Your Turn:
Use the joystick to scroll through the squares. Press the joystick on the piece you want to move.
Then scroll and find the square you want to move it to, and then press the joystick
again to place the selected piece there. To see what your valid moves are, move the
joystick over your selected piece.  If you want to CANCEL your move (i.e
change your mind and select another piece), select the piece you initially selected.
If you choose an invalid place to move to, a message will be displayed informing
you of the rule infringement, and you will be able to try again.

#### Checking:
If your opponent places you in check, a message will be displayed informing you
that you are in check. Your next move MUST get you out of the check. If you attempt
to make a move that doesn't get you out of check, a warning message will be
displayed, and the move will not be allowed. Once you get out of check, the game
proceeds as normal.

#### Checkmate/End Game:
When you or your opponent enters the check state, the program will automatically
check for a checkmate. If there is indeed a checkmate, the game will end and the
winner will be displayed. To play again, just press the reset button.

#### Castling:
If the rook and the king have not yet moved, the spaces are empty between them,
none of the in betweens or the new location of the king are in check,
you can castle. To perform the castling maneuver, select your king, and then
select the rook you want to castle with, and the move will be performed automatically.
Refer to https://en.wikipedia.org/wiki/Castling
for valid criteria for castling, if you are unfamiliar with this rule.

#### Promotion:
If one of your pawns reaches the opposite end of the board, it will automatically
be promoted to a queen.

#### En Passant:
If you match the correct criteria to perform an en passant, select the pawn you
want to perform the maneuver with, and then select the space behind the opposing pawn you
want to capture, to automatically perform the en passant maneuver.
Refer to https://en.wikipedia.org/wiki/En_passant
for valid criteria for an enpassant, if you are unfamiliar with this rule.


----
##### Note on special moves:
Special moves are not highlighted. You need to know when and where to
move your pieces

## File Layout / Description:
---
##### chess.cpp:
Has the main() function and is the main file.

##### chessfunctions.cpp:
Contains the crux of the functions, and is mainly where the game operates.
Contains functions that draw the board, draw pieces on the board, scroll through
the squares, move a piece and updates the menu with tips.

##### validmoves.cpp:
Contains functions that are used in chessfunctions.cpp to validate moves for each
piece type, check for obstruction in a potential move, check if the player is
in a check state, and a function that looks for a checkmate whenever someone is
in check.

##### specialcases.cpp:
Contains functions that check for, and perform the special moves of en passant,
castling and promotion.

##### lcd_image.cpp:
Allows the user to draw lcd images to the Arduino display

##### /icons:
This folder contains all the images of the chess piece and MUST be placed onto
the SD card.

## Acknowledgments:
--------------------------------------------------------------------------------
The images of the chess pieces were taken off of
https://commons.wikimedia.org/wiki/Category:PNG_chess_pieces/Standard_transparent
and modified in Photoshop to have different background colors, so that they
look transparent on the chess board.

The chess piece images were converted to lcd format using the bmptolcd tool posted
to the forum, originally developed by Chris Martin.

The lcd_image.cpp file was given to us during the Restaurant Finder assignment

The scroll() function was based off the joystick scrolling that was developed in class
for the Restaurant Finder assignment.
