#ifndef _MENU_H_
#define _MENU_H_

#include "common.h"
#include "Game.h"

class MenuOption {
public:
	string Text;
	int OptionValue;
	MenuOption() {
		Text = "";
		OptionValue = 0;
	}

	MenuOption(int ov, string text) {
		Text = text;
		OptionValue = ov;
	}
};

class Menu {
private:
	int m_ActiveOption;
	vector<MenuOption> m_MenuOptions;
	vector<Entity*> m_Entities;
	Renderer* m_Renderer;
	Window* m_Window;
	LevelBackground* m_Background;
	Camera* m_Camera;
	bool m_Running;
	double m_LastInputTime;
	double m_InputRate;

public:
	Menu();

	void SetupOptions() {
		m_MenuOptions.push_back(MenuOption(0, "Play Tutorial"));
		m_MenuOptions.push_back(MenuOption(1, "Play Game"));
		m_MenuOptions.push_back(MenuOption(2, "Exit"));
	}

	int Run();

	void RenderOptions();
	void ResetRenderer();
	void NextOption();
	void PreviousOption();
	void PrintGameState(GameState* state);
	void SaveGameState(GameState* state);

	void KillProgram() {
		m_Running = false;
	}

	void SetRenderer(Renderer* render) { m_Renderer = render; }
	void SetWindow(Window* window) { m_Window = window; }
};

#endif