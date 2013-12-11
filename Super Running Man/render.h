// SIT251 Assignment 1
// Mirko Velic - 900216058

#ifndef _RENDER_H_
#define _RENDER_H_
#include "game.h"

#define TP char(26) // transparent char

struct ObjectSize {
	int width, height;
};

void ClearScreen( HANDLE hConsole );
void PlaceCursor(HANDLE hConsole, const int x, const int y);
int GetConsoleWidth(HANDLE hConsole);
int GetConsoleHeight(HANDLE hConsole);
void Render(HANDLE hConsole, vector<GameObject>* gObjects);
ObjectSize GetObjectSize(GameObject gObject);

#endif
