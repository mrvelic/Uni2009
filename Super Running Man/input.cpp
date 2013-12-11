// SIT251 Assignment 1
// Mirko Velic - 900216058

#include "input.h"

// function to enable capturing input from the console
void HandleInput(HANDLE hInput) {
	DWORD fdwMode;
	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT; 
    SetConsoleMode(hInput, fdwMode);
}
