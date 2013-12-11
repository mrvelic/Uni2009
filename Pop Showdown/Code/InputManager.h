#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_
#include "common.h"
#include "Application.h"
#include "InputHandler.h"

extern Application mainApplication;

class InputManager {
private:
	InputHandler* m_ActiveHandler;
	bool m_HandlerSet;
	static InputManager* m_Instance;
public:
	static InputManager* Instance();
	void SetHandler(InputHandler* handler) { m_ActiveHandler = handler; m_HandlerSet = true; }
	int DispatchMessages(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);
	InputManager() {
		m_HandlerSet = false;
	}

	~InputManager() {
		delete m_Instance;
	}
};

#endif