#ifndef _VALIDMOVES_H
#define _VALIDMOVES_H

bool validateMove(int piecetomove, int selX, int selY, int boardtouse[][8]);

bool checkObstruction(int piece, int selX, int selY, int boardtouse[][8]);

void highlightValid(int pieceToMove);

void unhighlightValid(int pieceToMove);

bool checkOnBlack(int piece = 20);

bool checkOnWhite(int piece = 20);

#endif
