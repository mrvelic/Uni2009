#ifndef _APPLICATION_H_
#define _APPLICATION_H_
#include "Window.h"
#include "WindowOGL.h"
#include "Renderer.h"
#include "RenderOGL.h"
#include "Game.h"
#include "Menu.h"


class Application {
private:
	Window* m_Window;
	Renderer* m_Renderer;
	Game* m_Game;
	Config* m_Config;
	bool m_AppRunning;


public:
	Application() { m_AppRunning = false; }

	Game* GetGame() { return m_Game; }

	int Run() {
		m_Config = new Config();
		m_Renderer = new RenderOGL();
		
		m_Config->Active(true);
		m_Config->Title("Pop Showdown - Mirko Velic - 900216058");
		m_Config->WindowColourDepth(32);
		
		m_Config->WindowWidth(800);
		m_Config->WindowHeight(600);
		m_Config->GameFOV(45.0f);
		m_Config->WindowFullscreen(false);

		m_Window = new WindowOGL(m_Config);
		

		m_Window->CreateGameWindow();

		m_Renderer->SetWindow(m_Window);
		m_Renderer->BuildFont();

		m_AppRunning = true;

		Menu* mainMenu = new Menu();
		mainMenu->SetRenderer(m_Renderer);
		mainMenu->SetWindow(m_Window);
		int retval = mainMenu->Run();

		m_Window->KillGameWindow();

		return retval;
	}

	Window* GetWindow() { return m_Window; }
	bool IsRunning() { return m_AppRunning; }
	void IsRunning(bool running) { m_AppRunning = running; }

	~Application() {
		delete m_Window;
		delete m_Renderer;
		delete m_Config;
	}
};
#endif