#ifndef _GAMEINPUTHANDLER_H_
#define _GAMEINPUTHANDLER_H_
#include "InputHandler.h"
#include "Menu.h"

class MainMenuInputHandler: public InputHandler {
private:
	Menu* m_Menu;

public:
	MainMenuInputHandler() : InputHandler() {}

	MainMenuInputHandler(Menu* menu) : InputHandler() {
		m_Menu = menu;
	}

	virtual void OnQuit() {
		m_Menu->KillProgram();
	}

	virtual void OnKeyUp(unsigned int msg, WPARAM wParam) {
		m_Keys[wParam] = false;
	}

	virtual void OnKeyDown(unsigned int msg, WPARAM wParam) {
		m_Keys[wParam] = true;
	}

	~MainMenuInputHandler() {}
};

#endif