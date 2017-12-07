#ifndef _SPECIALCASES_H
#define _SPECIALCASES_H

extern bool p1_kingMoved;
extern bool p1_leftRookmoved;
extern bool p1_rightRookmoved;

extern bool p2_kingMoved;
extern bool p2_leftRookmoved;
extern bool p2_rightRookmoved;

extern bool p1_pawn2spaces[8];
extern bool p2_pawn2spaces[8];


void promote_to_Queen(int x, int y);

bool checkSpecialcases(int x, int y, int piece);

bool castling(int x, int y);

#endif
