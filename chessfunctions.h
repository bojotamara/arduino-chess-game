#ifndef _CHESSFUNCTIONS_H
#define _CHESSFUNCTIONS_H

extern int currentplayer;

void drawBoard();

void fillBoardArray();

void emptySquare(int squarex, int squarey);

void drawPiece(int squarex, int squarey, int piecetype);

void drawArray();

void unhighlightSquare(int squarex, int squarey);

void highlightSquare(int squarex, int squarey, uint16_t bordercolor= 0xFFFF);

void scroll();

void movePiece(int oldx, int oldy, int pieceToMove);

void moveMode();

void dispCurrentPlayer();

void dispTips(String tip);

#endif
