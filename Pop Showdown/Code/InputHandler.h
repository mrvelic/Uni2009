#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_
#include "common.h"

class InputHandler {
protected:
	bool* m_Keys;
	int m_MouseX, m_MouseY;
public:
	InputHandler() { 
		m_Keys = new bool[256];
		ResetState();
	}

	void ResetState() {
		for(int i = 0; i < 256; i++) {
			m_Keys[i] = false;
		}
		m_MouseX = 0;
		m_MouseY = 0;
	}

	virtual void OnKeyUp(unsigned int msg, WPARAM wParam) {}
	virtual void OnKeyDown(unsigned int msg, WPARAM wParam) {}
	virtual void OnQuit() {}
	int MouseX() { return m_MouseX; }
	int MouseY() { return m_MouseY; }
	void MouseX(int x) { m_MouseX = x; }
	void MouseY(int y) { m_MouseY = y; }
	bool* GetKeyState() { return m_Keys; }
	virtual ~InputHandler() {
		delete m_Keys;
	}
};


#endif