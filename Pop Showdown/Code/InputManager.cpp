#include "InputManager.h"

InputManager* InputManager::m_Instance = NULL;

InputManager* InputManager::Instance() {
	if(m_Instance == NULL) {
		m_Instance = new InputManager();
	}
	return m_Instance;
}

int InputManager::DispatchMessages(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam) {
	if(m_HandlerSet) {
		switch(msg) {
		case WM_CLOSE: 
			{
				m_ActiveHandler->OnQuit();
				return 0;
			}

		case WM_KEYUP:
			{
				m_ActiveHandler->OnKeyUp(msg, wParam);
				return 0;
			}

		case WM_KEYDOWN:
			{
				m_ActiveHandler->OnKeyDown(msg, wParam);
				return 0;
			}
		}

		POINT cursorPos;
		GetCursorPos(&cursorPos);
		m_ActiveHandler->MouseX(cursorPos.x);
		m_ActiveHandler->MouseY(cursorPos.y);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}