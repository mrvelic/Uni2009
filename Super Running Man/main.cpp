// SIT251 Assignment 1
// Mirko Velic - 900216058

#include "common.h"
#include "render.h"
#include "game.h"
#include "input.h"
#include "menu.h"

int main(int argc, char* argv[])
{
	// input / output handles
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

	// seed random number generator
	srand((int)time(NULL));

	// set the title of the console
	SetConsoleTitle(TEXT("SIT251 Assignment 1"));

	// clear out screen
	ClearScreen(hConsole);
	PlaceCursor(hConsole, 0, 0);

	// attempt to make cursor invisible (doesnt work for some reason)
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursorInfo);

	// set the screen size (removes scrollbar etc)
	COORD newScreenSize = { GetConsoleWidth(hConsole), GetConsoleHeight(hConsole) * 2 }; 
	SMALL_RECT newWindowSize;
	newWindowSize.Left = 0;
	newWindowSize.Top = 0;
	newWindowSize.Right = GetConsoleWidth(hConsole) - 1;
	newWindowSize.Bottom = (GetConsoleHeight(hConsole) * 2) - 1;
	SetConsoleScreenBufferSize(hConsole, newScreenSize);
	SetConsoleWindowInfo(hConsole, TRUE, &newWindowSize);

	// capture input from console
	HandleInput(hInput);

	// run the game itself!
	int retCode = RunMenu(hConsole, hInput);

	ClearScreen(hConsole);

	return retCode;
}

