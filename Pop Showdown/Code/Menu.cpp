#include "Menu.h"
#include "MainMenuInputHandler.h"
#include "InputManager.h"

Menu::Menu() {
	m_Running = true;
	SetupOptions();
	m_ActiveOption = 0;
	m_InputRate = 70;
	m_LastInputTime = Timer::GetCurrentMilli();
	m_Camera = new Camera(0.0f, 0.0f, -2000.0f);
	m_Background = new LevelBackground(new Colour(217.0f, 53.0f, 157.0f), new Colour(0, 0, 0));
	Sprite* logoSprite = new Sprite("game_heading.png");
	Entity* logoEntity = new Entity();
	logoEntity->SetSprite(logoSprite);
	logoEntity->Y(400.0f);
	logoSprite->Scale(1000.0f);
	m_Entities.push_back(logoEntity);
}

int Menu::Run() {
	MainMenuInputHandler* inputHandler = new MainMenuInputHandler(this);

	Game* game;
	GameState* gameResult;
	bool gamePlayed = false;
	ResetRenderer();

	while(m_Running) {
		InputManager::Instance()->SetHandler(inputHandler);
		if(GameInput::DispatchMessages()) {
			double currentTime = Timer::GetCurrentMilli();
			double timeDifference = currentTime - m_LastInputTime;

			
			if(inputHandler->GetKeyState()[VK_RETURN]) {
				switch(m_ActiveOption) {
				case 0:
					game = new Game(m_Renderer, m_Window);
					game->SetInstance(game);
					gameResult = game->Run("tutorial.txt");
					SaveGameState(gameResult);
					inputHandler->ResetState();
					gamePlayed = true;
					ResetRenderer();
					delete game;
					break;
				case 1:
					game = new Game(m_Renderer, m_Window);
					game->SetInstance(game);
					gameResult = game->Run("level1.txt");
					SaveGameState(gameResult);
					inputHandler->ResetState();
					gamePlayed = true;
					ResetRenderer();
					delete game;
					break;
				case 2:
					m_Running = false;
					break;
				}
			}

			if(inputHandler->GetKeyState()[VK_ESCAPE]) {
				break;
			}

			if(timeDifference > m_InputRate) {
				if(inputHandler->GetKeyState()[VK_DOWN]) {
					NextOption();
				}

				if(inputHandler->GetKeyState()[VK_UP]) {
					PreviousOption();
				}

				m_LastInputTime = currentTime;
			}


			m_Renderer->Draw();
			RenderOptions();
			if(gamePlayed) PrintGameState(gameResult);
			m_Window->WinSwapBuffers();
		}
	}

	return 0;
}

void Menu::PrintGameState(GameState* state) {
	m_Renderer->DrawText(-400.0f, -300.0f, Colour(98, 241, 64), "Last Game:");

	stringstream killed, fired, levels;

	killed << "You killed " << state->EnemiesKilled << " enemies!";
	fired << "You fired " << state->ShotsFired << " shots!";
	levels << "You played through " <<  state->LevelsCompleted << " levels!";

	m_Renderer->DrawText(-400.0f, -350.0f, Colour(98, 241, 64), killed.str());
	m_Renderer->DrawText(-400.0f, -400.0f, Colour(98, 241, 64), fired.str());
	m_Renderer->DrawText(-400.0f, -450.0f, Colour(98, 241, 64), levels.str());

	m_Renderer->DrawText(-400.0f, -500.0f, Colour(98, 241, 64), "Score saved to score.txt!");
}


void Menu::SaveGameState(GameState* state) {
	stringstream killed, fired, levels;

	killed << "You killed " << state->EnemiesKilled << " enemies!";
	fired << "You fired " << state->ShotsFired << " shots!";
	levels << "You played through " <<  state->LevelsCompleted << " levels!";

	ofstream file("score.txt");
	if(file.is_open()) {
		file << killed.str() << endl;
		file << fired.str() << endl;
		file << levels.str() << endl;
		file.close();
	}
}



void Menu::RenderOptions() {
	float baseX = -170.0f;
	float baseY = 0.0f;
	for(int o = m_MenuOptions.size() - 1; o >= 0; o--) {
		MenuOption co = m_MenuOptions[o];
		Colour itemColour = Colour(98, 241, 64); 
		if(co.OptionValue == m_ActiveOption) itemColour = Colour(249, 255, 92);
		m_Renderer->DrawText(baseX, baseY - (100.0f * o), itemColour, co.Text);
	}

	m_Renderer->DrawText(-280.0f, -700.0f, Colour(98, 241, 64), "Mirko Velic - 900216058");
	m_Renderer->DrawText(-350.0f, -800.0f, Colour(98, 241, 64), "Actual gameplay is incomplete!");
}

void Menu::NextOption() {
	int maxOption = m_MenuOptions.size() -1;
	int newOption = m_ActiveOption + 1;
	if(newOption > maxOption) newOption = 0;
	m_ActiveOption = newOption;
}

void Menu::PreviousOption() {
	int maxOption = m_MenuOptions.size() -1;
	int newOption = m_ActiveOption - 1;
	if(newOption < 0) newOption = maxOption;
	m_ActiveOption = newOption;
}

void Menu::ResetRenderer() {
	m_Renderer->ResetRenderer();
	m_Renderer->SetCamera(m_Camera);
	m_Renderer->SetBackground(m_Background);
	m_Renderer->SetEntities(&m_Entities);
}