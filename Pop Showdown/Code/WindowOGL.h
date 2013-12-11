#ifndef _WINDOW_OGL_H
#define _WINDOW_OGL_H
#include "common.h"
#include "Window.h"
#include "WinProc.h"
class WindowOGL : public Window {
private:
	HGLRC m_RC;
public:
	WindowOGL();
	WindowOGL(Config* config) {
		m_WindowRect.left = (long)0;
		m_WindowRect.right = (long)config->WindowWidth();
		m_WindowRect.top = (long)0;
		m_WindowRect.bottom = (long)config->WindowHeight();
		m_Config = config;
	}

	virtual bool CreateGameWindow() {
		m_hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
		m_WC.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		m_WC.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
		m_WC.cbClsExtra		= 0;									// No Extra Window Data
		m_WC.cbWndExtra		= 0;									// No Extra Window Data
		m_WC.hInstance		= m_hInstance;							// Set The Instance
		m_WC.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
		m_WC.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		m_WC.hbrBackground	= NULL;									// No Background Required For GL
		m_WC.lpszMenuName		= NULL;									// We Don't Want A Menu
		m_WC.lpszClassName	= TEXT("GameOpenGL");								// Set The Class Name

		if (!RegisterClass(&m_WC))									// Attempt To Register The Window Class
		{
			MessageBox(NULL,TEXT("Failed To Register The Window Class."),TEXT("ERROR"),MB_OK|MB_ICONEXCLAMATION);
			return FALSE;											// Return FALSE
		}
	
		if (m_Config->WindowFullscreen())												// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;								// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= m_Config->WindowWidth();				// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= m_Config->WindowHeight();				// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= m_Config->WindowColourDepth();					// Selected Bits Per Pixel
			dmScreenSettings.dmFields= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			{
				// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
				if (MessageBoxA(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", m_Config->Title().c_str(), MB_YESNO | MB_ICONEXCLAMATION)==IDYES)
				{
					m_Config->WindowFullscreen(false);		// Windowed Mode Selected.  Fullscreen = FALSE
				}
				else
				{
					// Pop Up A Message Box Letting User Know The Program Is Closing.
					MessageBoxA(NULL, "Program Will Now Close.", m_Config->Title().c_str(), MB_OK | MB_ICONSTOP);
					return FALSE;									// Return FALSE
				}
			}
		}

		if (m_Config->WindowFullscreen()) {											// Are We Still In Fullscreen Mode?
			m_dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
			m_dwStyle = WS_POPUP;										// Windows Style
			ShowCursor(false);										// Hide Mouse Pointer
		} else {
			m_dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			m_dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Styles
		}

		AdjustWindowRectEx(&m_WindowRect, m_dwStyle, false, m_dwExStyle);		// Adjust Window To True Requested Size

		// Create The Window
		if (!(m_hWND=CreateWindowExA(	m_dwExStyle,							// Extended Style For The Window
									"GameOpenGL",							// Class Name
									m_Config->Title().c_str(),								// Window Title
									m_dwStyle |							// Defined Window Style
									WS_CLIPSIBLINGS |					// Required Window Style
									WS_CLIPCHILDREN,					// Required Window Style
									0, 0,								// Window Position
									m_WindowRect.right - m_WindowRect.left,	// Calculate Window Width
									m_WindowRect.bottom - m_WindowRect.top,	// Calculate Window Height
									NULL,								// No Parent Window
									NULL,								// No Menu
									m_hInstance,							// Instance
									NULL)))								// Dont Pass Anything To WM_CREATE
		{
			KillGameWindow();								// Reset The Display
			MessageBoxA(NULL, "Window Creation Error.", m_Config->Title().c_str(), MB_OK | MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			m_Config->WindowColourDepth(),										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
	
		if (!(m_hDC = GetDC(m_hWND)))							// Did We Get A Device Context?
		{
			KillGameWindow();								// Reset The Display
			MessageBoxA(NULL, "Can't Create A GL Device Context.", m_Config->Title().c_str(), MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return FALSE
		}

		if (!(m_PixelFormat = ChoosePixelFormat(m_hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			KillGameWindow();								// Reset The Display
			MessageBoxA(NULL, "Can't Find A Suitable PixelFormat.", m_Config->Title().c_str(),MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return FALSE
		}

		if(!SetPixelFormat(m_hDC, m_PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
		{
			KillGameWindow();								// Reset The Display
			MessageBoxA(NULL, "Can't Set The PixelFormat.", m_Config->Title().c_str(), MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return FALSE
		}

		if (!(m_RC=wglCreateContext(m_hDC)))				// Are We Able To Get A Rendering Context?
		{
			KillGameWindow();								// Reset The Display
			MessageBoxA(NULL, "Can't Create A GL Rendering Context.", m_Config->Title().c_str(), MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return FALSE
		}

		if(!wglMakeCurrent(m_hDC,m_RC))					// Try To Activate The Rendering Context
		{
			KillGameWindow();								// Reset The Display
			MessageBoxA(NULL, "Can't Activate The GL Rendering Context.", m_Config->Title().c_str(), MB_OK | MB_ICONEXCLAMATION);
			return false;								// Return FALSE
		}

		ShowWindow(m_hWND,SW_SHOW);						// Show The Window
		SetForegroundWindow(m_hWND);						// Slightly Higher Priority
		SetFocus(m_hWND);									// Sets Keyboard Focus To The Window
		ReSizeGLScene(m_Config->WindowWidth(), m_Config->WindowHeight());					// Set Up Our Perspective GL Screen

		if (!InitGL())									// Initialize Our Newly Created GL Window
		{
			KillGameWindow();								// Reset The Display
			//MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return false;								// Return FALSE
		}

		return TRUE;									// Success
	}

	virtual void KillGameWindow() { KillGLWindow(); }

	void ReSizeGLScene(int width, int height) {
		if (height == 0) height = 1;

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Calculate The Aspect Ratio Of The Window
		GLfloat	newAspect = (GLfloat)width / (GLfloat)height;
		gluPerspective(m_Config->GameFOV(), newAspect, 0.1f, 100.0f);
		//glOrtho(0, width, height, 0, -1.0f, 1.0f);
		//glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	virtual void ResizeWindow(int width, int height) {
		ReSizeGLScene(width, height);
	}

	bool InitGL() {
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glDisable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		return true;										// Initialization Went OK
	}

	void KillGLWindow() {
		if (m_Config->WindowFullscreen())										// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
			ShowCursor(TRUE);								// Show Mouse Pointer
		}

		if (m_RC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				//MessageBox(NULL,TEXT("Release Of DC And RC Failed.","SHUTDOWN ERROR"),MB_OK | MB_ICONINFORMATION);
			}

			if (!wglDeleteContext(m_RC))						// Are We Able To Delete The RC?
			{
				//MessageBox(NULL,TEXT("Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			m_RC=NULL;										// Set RC To NULL
		}

		if (m_hDC && !ReleaseDC(m_hWND, m_hDC))					// Are We Able To Release The DC
		{
									// Set DC To NULL
		}

		if (m_hWND && !DestroyWindow(m_hWND))					// Are We Able To Destroy The Window?
		{
										// Set hWnd To NULL
		}

		if (!UnregisterClass(TEXT("GameOpenGL"),m_hInstance))			// Are We Able To Unregister Class
		{
								// Set hInstance To NULL
		}

	}

	virtual Vertex3f GetWorldMouseCoord(int x, int y) {
		GLint viewport[4];
		GLdouble modelview[16];
		GLdouble projection[16];
		GLfloat winX, winY, winZ;
		GLdouble posX, posY, posZ;

		glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
		glGetDoublev( GL_PROJECTION_MATRIX, projection );
		glGetIntegerv( GL_VIEWPORT, viewport );

		winX = (float)x;
		winY = (float)viewport[3] - (float)y;
		glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

		gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		return Vertex3f((float)posX, (float)posY, (float)posZ);
	}

	~WindowOGL() { KillGLWindow(); }
};


#endif