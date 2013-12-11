#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "common.h"
#include "Polygon.h"
#include "Config.h"
#include "Input.h"

class Window {
protected:
	HDC m_hDC;
	HWND m_hWND;
	HINSTANCE m_hInstance;
	unsigned int m_PixelFormat;
	WNDCLASS m_WC;
	DWORD m_dwExStyle;
	DWORD m_dwStyle;
	RECT m_WindowRect;

	Config* m_Config;

public:

	virtual bool CreateGameWindow() = 0;
	virtual void KillGameWindow() = 0;
	virtual void ResizeWindow(int width, int height) = 0;
	virtual Vertex3f GetWorldMouseCoord(int x, int y) = 0;
	void WinSwapBuffers() { SwapBuffers(m_hDC); }

	HDC GetHDC() { return m_hDC; }
};

#endif