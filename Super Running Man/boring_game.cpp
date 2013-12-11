// SIT251 Assignment 1
// Mirko Velic - 900216058

#include "common.h"
#include "game.h"
#include "render.h"
#include "boring_game.h"

int RunBoringGame(HANDLE hConsole, HANDLE hInput) {
	// basics..
	vector<GameObject> vGObjects;
	wstringstream playerStr, enemyStr;
	int enemyDirection = 1;
	// Question 4 (multiple characters)
	playerStr << TP << "^" << TP <<endl
			  << "^" << TP << "^";
	GameObject player = { 40, 49, 0, 1, playerStr.str(), FOREGROUND_RED};

	enemyStr << "x x";
	GameObject enemy = { 0, 0, 0, 1, enemyStr.str(), BACKGROUND_GREEN | FOREGROUND_BLUE };

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
					// Question 2
					switch(last_events[i].Event.KeyEvent.uChar.AsciiChar)
					{
					case 'a':
						player.x--;
						break;

					case 'd':
						player.x++;
						break;

					case ' ': // Question 3
						if(player.x == enemy.x) enemy.attributes = BACKGROUND_RED;
						break;

					case char(27):
						return 0;
						break;
					}
				}
			}
		}

		// enemy movement (Question 1)
		if(enemy.x == 79) enemyDirection = 0;
		if(enemy.x == 0) enemyDirection = 1;
		if(enemyDirection == 0) enemy.x--;
		if(enemyDirection == 1) enemy.x++;

		vGObjects.clear();
		
		vGObjects.push_back(enemy);
		vGObjects.push_back(player);

		Render(hConsole, &vGObjects);

		Sleep(50);
	}
	return 0;
}