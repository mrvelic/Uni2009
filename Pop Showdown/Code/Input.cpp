#include "common.h"
#include "Input.h"

bool GameInput::DispatchMessages() {
	MSG msg;
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		return false;
	} 
	return true;
}