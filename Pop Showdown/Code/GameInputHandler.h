#ifndef _GAMEINPUTHANDLER_H_
#define _GAMEINPUTHANDLER_H_
#include "InputHandler.h"
#include "Game.h"

class GameInputHandler: public InputHandler {
private:
	Game* m_Game;

public:
	GameInputHandler() : InputHandler() {}

	GameInputHandler(Game* game) : InputHandler() {
		m_Game = game;
	}

	virtual void OnQuit() {
		m_Game->StopGame();
	}

	virtual void OnKeyUp(unsigned int msg, WPARAM wParam) {
		m_Keys[wParam] = false;
	}

	virtual void OnKeyDown(unsigned int msg, WPARAM wParam) {
		m_Keys[wParam] = true;
	}

	~GameInputHandler() {}
};

#endif