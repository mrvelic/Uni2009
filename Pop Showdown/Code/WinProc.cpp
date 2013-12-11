#include "common.h"
#include "main.h"
#include "WinProc.h"
#include "InputManager.h"

extern Application mainApplication;

// horrible winproc crap
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if(mainApplication.IsRunning() == true) {
		switch (uMsg)									// Check For Windows Messages
		{
			case WM_ACTIVATE:							// Watch For Window Activate Message
			{
				return 0;								// Return To The Message Loop
			}

			case WM_SYSCOMMAND:							// Intercept System Commands
			{
				switch (wParam)							// Check System Calls
				{
					case SC_SCREENSAVE:					// Screensaver Trying To Start?
					case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return 0;							// Prevent From Happening
				}
				break;									// Exit
			}

			case WM_SIZE:								// Resize The OpenGL Window
			{
				mainApplication.GetWindow()->ResizeWindow(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
				return 0;								// Jump Back
			}

			default:
			{
				return InputManager::Instance()->DispatchMessages(hWnd, uMsg, wParam, lParam);
			}
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);



}