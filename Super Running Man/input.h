// SIT251 Assignment 1
// Mirko Velic - 900216058

#ifndef _INPUT_H_
#define _INPUT_H_
#include "common.h"

struct MouseState {
	bool buttonDown, leftButton, rightButton;
};

void HandleInput(HANDLE hInput);

#endif