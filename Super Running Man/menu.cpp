// SIT251 Assignment 1
// Mirko Velic - 900216058

#include "common.h"
#include "game.h"
#include "render.h"
#include "menu.h"
#include "boring_game.h"

int RunMenu(HANDLE hConsole, HANDLE hInput) {
	vector<GameObject> vGObjects;
	vector<GameObject> vUI;
	vector<GameObject> vBackground;
	vector<GameObject> vDialogs;
	vector<WORD> colourTable;
	Score gameResult;

	// colours for background
	// does it over and over incase of increased fractal complexity
	for(int i = 0; i < 20; i++) {
		colourTable.push_back(FOREGROUND_INTENSITY);
		colourTable.push_back(BACKGROUND_RED | BACKGROUND_GREEN);
		colourTable.push_back(BACKGROUND_RED | BACKGROUND_BLUE);
		colourTable.push_back(BACKGROUND_INTENSITY);
		colourTable.push_back(BACKGROUND_RED);
		colourTable.push_back(BACKGROUND_BLUE);
		colourTable.push_back(BACKGROUND_GREEN);
	}

	// screen size
	int screenWidth = GetConsoleWidth(hConsole);
	int screenHeight = GetConsoleHeight(hConsole);

	// vars for fractal background
	BackgroundInfo bgInfo = { -0.5f, -0.5f, 0.5f, 0.7f, 0.4f, 1.0f, 0.4f, 0.01f, 0, screenWidth, screenHeight };

	// main UI elements
	int headingWidth = (screenWidth - 20);
	wstringstream wssHeading;
	wssHeading << setw(headingWidth) << " " << endl
				<< setw(headingWidth) << " " << endl
				<< setw(headingWidth) << left << "               Super Running Man vs Zombies!" << endl // dodgy way to center the text xD
				<< setw(headingWidth) << " " << endl
				<< setw(headingWidth) << " " << endl;

	GameObject goHeading = { (screenWidth / 2), 6, 0, 1, wssHeading.str(), BACKGROUND_BLUE | FOREGROUND_INTENSITY};
	vUI.push_back(goHeading);

	wstringstream wssName;
	wssName << setw(headingWidth) << " " << endl
				<< setw(headingWidth) << left << "                   Mirko Velic - 900216058" << endl // dodgy way to center the text xD
				<< setw(headingWidth) << " " << endl;

	GameObject goName = { (screenWidth / 2), (screenHeight - 6), 0, 1, wssName.str(), BACKGROUND_BLUE | FOREGROUND_INTENSITY};
	vUI.push_back(goName);

	wstringstream wssMenu;
	wssMenu << "+-------------------------------+" << endl
			<< "|                               |" << endl
			<< "| [1]  Play Game                |" << endl
			<< "| [2]  Instructions             |" << endl
			<< "| [3]  Questions 1 - 3          |" << endl
			<< "| [Q]  Quit Game                |" << endl
			<< "|                               |" << endl
			<< "+-------------------------------+" << endl;

	GameObject goMenu = { (screenWidth / 2), (screenHeight / 2), 0, 1, wssMenu.str(), FOREGROUND_INTENSITY};
	vUI.push_back(goMenu);

	for(;;) {
		DWORD events_read;
		INPUT_RECORD last_events[128];

		// check for events...
		PeekConsoleInput(hInput, last_events, 128, &events_read);
		// if there are some events lets process them
		if(events_read > 0) {
			// read the actual events so they arent in the buffer anymore
			ReadConsoleInput(hInput, last_events, 128, &events_read);

			// loop over each event
			for(unsigned int i = 0; i < events_read; i++) {
				// if a key event
				if(last_events[i].EventType == KEY_EVENT) {
					switch(last_events[i].Event.KeyEvent.uChar.AsciiChar)
					{
					case '1': // run game
						gameResult = RunGame(hConsole, hInput);
						ShowGameResult(&vDialogs, &gameResult);
						break;
					case '2': // instructions
						ShowInstructions(&vDialogs);
						break;
					case '3':
						RunBoringGame(hConsole, hInput);
						break;
					case 'q': // quit game
						return 0;
						break;
					case char(13): // enter key (dismiss all dialogs)
						DismissDialogs(&vDialogs);
						break;
					}
				}
			}
		}

		

		// generate the magical fractal background frame :D :D ...
		GenerateBackgroundFrame(&vBackground, &colourTable, &bgInfo);

		// render
		vGObjects.clear();
		
		AppendGObjects(&vGObjects, &vBackground);
		AppendGObjects(&vGObjects, &vUI);
		AppendGObjects(&vGObjects, &vDialogs);
		

		// for some reason the centering in the renderer is pure evil with large amounts of text
		// so im gonna do it here :/
		CentreObjects(&vGObjects);

		Render(hConsole, &vGObjects);

		Sleep(20);
	}

}

void ShowInstructions(vector<GameObject>* vDialogs) {
	wstringstream insText;
	insText << "+---------------------------------------------------------------------------+" << endl
			<< "|                               Instructions                                |" << endl
			<< "+---------------------------------------------------------------------------+" << endl
			<< "|                                                                           |" << endl
			<< "| Press the left or right mouse buttons to fire your guns.                  |" << endl
			<< "| You can use both buttons at once to use both guns at the same time.       |" << endl
			<< "|                                                                           |" << endl
			<< "| The zombies will come at you in waves, kill them all and survive to win!  |" << endl
			<< "|                                                                           |" << endl
			<< "| Press [ESC] at any time to exit.                                          |" << endl
			<< "|                                                                           |" << endl
			<< "|                                             Press [ENTER] to close.       |" << endl
			<< "+---------------------------------------------------------------------------+";

	CreateMenuDialog(vDialogs, 40, 25, insText.str());
}

void ShowGameResult(vector<GameObject>* vDialogs, Score* gameResult) {
	wstringstream insText;
	wstringstream ssResult;

	// calculate the accuracy of the shots
	double accuracy = 0.0f;
	if(gameResult->enemiesKilled != 0 && gameResult->shotsFired != 0) {
		accuracy = ((double)gameResult->enemiesKilled / (double)gameResult->shotsFired) * 100.0f;
	}
	// accuracy shouldn't be over 100%
	if(accuracy > 100.0f) accuracy = 100.0f;

	insText << "+-----------------------------------+" << endl
			<< "|              Results              |" << endl
			<< "+-----------------------------------+" << endl
			<< "|                                   |" << endl
			<< "| Waves Survived:" << setw(17) << gameResult->wave << "  |" << endl
			<< "| Killed:        " << setw(17) << gameResult->enemiesKilled << "  |" << endl
			<< "| Shots Fired:   " << setw(17) << gameResult->shotsFired << "  |" << endl
			<< "| Accuracy:      " << setw(17) << setprecision(3) << accuracy << "% |" << endl
			<< "|                                   |" << endl
			<< "|           Press [ENTER] to close. |" << endl
			<< "+-----------------------------------+";

	CreateMenuDialog(vDialogs, 40, 25, insText.str());
}

void GenerateBackgroundFrame(vector<GameObject>* pixels, vector<WORD>* colourTable, BackgroundInfo* bgInfo) {
	// fractal "pixel" position vars
	float x, y;

	// number of fractal "pixels" compared to screen pixels
	float dx = (bgInfo->right - bgInfo->left)/(float)bgInfo->screenWidth;
	float dy = (bgInfo->bottom - bgInfo->top)/(float)bgInfo->screenHeight;

	// clear previous set of "pixels"
	pixels->clear();

	// for each fractal "pixel"
	for(int i = 0;i < bgInfo->screenWidth; i++){
		for(int j = 0;j < bgInfo->screenHeight; j++){
			// find out actual pixel we want to generate for the fractal relative to screen size
			x = (float)(bgInfo->left + (dx/2.0 + i*dx));
			y = (float)(bgInfo->top + (dy/2.0 + j*dy));

			// num iterations
			int itr = 0;

			// required complex numbers
			complex<float> Z(x, y);
			complex<float> C(bgInfo->animStep, bgInfo->animStep);

			// fractal caluclation
			while((abs(Z) < 2) && (itr < 4)) {
				Z = ((Z*Z) + C);
				itr++;
			}

			// figure out real screen "pixels"
			int sX = (int)((x - bgInfo->left) / dx);
			int sY = (int)((y - bgInfo->top) / dy);

			// avoid divide by zero errors
			if(itr == 0) itr++;

			// create object with the colour and place it on the screen
			wstringstream block;
			block << " ";
			GameObject curPixel = {sX, sY, 0, 0, block.str(), colourTable->at((colourTable->size() % itr))};
			pixels->push_back(curPixel);
		}
	}

	// change animation direction
	if(bgInfo->animStep >= bgInfo->maxAnim) bgInfo->animDirection = 0;
	if(bgInfo->animStep <= bgInfo->minAnim) bgInfo->animDirection = 1;

	// increment or decrement animation based on direction
	if(bgInfo->animDirection == 1) bgInfo->animStep += bgInfo->stepAmount;
	if(bgInfo->animDirection == 0) bgInfo->animStep -= bgInfo->stepAmount;
}

void CreateMenuDialog(vector<GameObject>* vDialogs, int x, int y, wstring text) {
	GameObject dialog = {x, y, 0, 1, text, FOREGROUND_INTENSITY};
	vDialogs->push_back(dialog);
}

// removes all dialogs
void DismissDialogs(vector<GameObject>* vDialogs) {
	for(unsigned int i = 0; i < vDialogs->size(); i++) {
		vDialogs->erase(vDialogs->begin() + i);
	}
}

// function to centre before renderer, for performance :/
void CentreObjects(vector<GameObject>* vObjects) {
	for(unsigned int i = 0; i < vObjects->size(); i++) {
		if(vObjects->at(i).drawFromCentre == 1) {
			ObjectSize objectSize = GetObjectSize(vObjects->at(i));
			vObjects->at(i).x = vObjects->at(i).x - (objectSize.width / 2);
			vObjects->at(i).y = vObjects->at(i).y - (objectSize.height / 2);
			vObjects->at(i).drawFromCentre = 0;
		}
	}
}