// SIT251 Assignment 1
// Mirko Velic - 900216058

// This file is Question 5

#include "common.h"
#include "game.h"
#include "render.h"

Score RunGame(HANDLE hConsole, HANDLE hInput) {
	// storage! :D
	vector<GameObject> vGObjects;
	vector<GameObject> vPlayers;
	vector<GameObject> vEnemies;
	vector<GameObject> vProjectiles;
	vector<GameObject> vUI;

	// animation related
	vector<GameObject> vEffects;
	vector<Animation> vAnimations;

	// game state
	Score scoreTracker = {0, 0, 0, 0, 100.0f, 0, true};
	MouseState mouseState = {false, false, false};

	// screen size
	int screenWidth = GetConsoleWidth(hConsole);
	int screenHeight = GetConsoleHeight(hConsole);

	// game config
	WaveConfig waveConfig;
	Boundry enemySpawnBoundry;
	Boundry enemyMovementBoundry;

	enemySpawnBoundry.left = 1;
	enemySpawnBoundry.top = 1;
	enemySpawnBoundry.right = screenWidth - 2;
	enemySpawnBoundry.bottom = 30;

	enemyMovementBoundry.left = 0;
	enemyMovementBoundry.top =  -32;
	enemyMovementBoundry.right = screenWidth - 1;
	enemyMovementBoundry.bottom = screenHeight - 1;

	waveConfig.startingEnemies = 200;
	waveConfig.damageRate = 5.0f;
	waveConfig.defaultTime = 30;
	waveConfig.enemiesPerWave = 200;
	waveConfig.enemyMovementSpeed = 5; // the lower the faster (2 is fastest)
	waveConfig.hitChance = 4; // the higher the less likely (2 is most likely)
	waveConfig.timeAddedPerWave = 0;

	// create UI
	CreateUI(hConsole, &vUI);

	// create initial enemies
	CreateEnemies(hConsole, &vEnemies, enemySpawnBoundry, waveConfig.enemiesPerWave);
	MoveEnemiesUp(&vEnemies, 30);

	// create player
	CreatePlayer(&vPlayers);

	// begin game loop!
	for(;;) {
		CheckGameState(hConsole, &vEnemies, &scoreTracker, &waveConfig, enemySpawnBoundry);
		if(scoreTracker.gameStarted) {
			// process input
			ProcessInput(hInput, &vPlayers, &vProjectiles, &mouseState, &scoreTracker);

			// Updates
			UpdateEnemies(hConsole, &vEnemies, &waveConfig, enemyMovementBoundry, vPlayers[0]); // move enemies around
			UpdateProjectiles(&vProjectiles); // updates projectile positions
			CheckKills(hConsole, &vProjectiles, &vEnemies, &vAnimations, &scoreTracker); // checks for kills
			CheckPlayerHit(&vPlayers, &vEnemies, &scoreTracker, &waveConfig);
			UpdateScore(&vUI, scoreTracker); // updates the score on the display
			UpdateAnimations(&vAnimations, &vEffects); // update the animation steps
		
		
			// this below just builds a larger list of objects to send to the renderer
			vGObjects.clear(); // clear out the old objects first
		
			AppendGObjects(&vGObjects, &vEnemies); // append the enemies
			AppendGObjects(&vGObjects, &vProjectiles); // append the projectiles
			AppendGObjects(&vGObjects, &vPlayers); // append the players
			AppendGObjects(&vGObjects, &vEffects); // append the effects (current animation steps etc)
			AppendGObjects(&vGObjects, &vUI); // append the UI components last

			// render the actual objects :D
			Render(hConsole, &vGObjects);
		} else {
			break;
		}

		// sleep for 20ms
		// I would have done proper timing, but didn't have enough time (ironically)
		Sleep(20);
	}

	return scoreTracker;
}

// actual game logic :D
void CheckGameState(HANDLE hConsole, vector<GameObject>* vEnemies, Score* scoreTracker, WaveConfig* waveConfig, Boundry enemyBoundry) {
	if(scoreTracker->health <= 0.0f) {
		scoreTracker->gameResult = GR_NOHEALTH; //ran out of health
		scoreTracker->gameStarted = false;
	}

	/*if(scoreTracker->wave > 9) {
		scoreTracker->gameResult = GR_GAMEFINISHED;
		scoreTracker->gameStarted = false;
	}*/

	// game stuff

	// no moar enemiezzz
	if(vEnemies->size() == 0) {
		waveConfig->startingEnemies += waveConfig->enemiesPerWave;
		if(waveConfig->hitChance > 2) waveConfig->hitChance--;
		if(waveConfig->enemyMovementSpeed > 2) waveConfig->enemyMovementSpeed--;
		CreateEnemies(hConsole, vEnemies, enemyBoundry, waveConfig->startingEnemies);
		MoveEnemiesUp(vEnemies, 30);
		scoreTracker->wave++;
	}

}

// function to add one set of objects to another
void AppendGObjects(vector<GameObject>* vGObjects, vector<GameObject>* vAppendObjects) {
	for(unsigned int i = 0; i < vAppendObjects->size(); i++) {
		vGObjects->push_back(vAppendObjects->at(i));
	}
}

void CreatePlayer(vector<GameObject>* vPlayers) {
	// create player
	wstringstream playerString;
	playerString << '+' << wchar_t(9787) << '+';
	GameObject goPlayer = { 40, 25, 0, 1, playerString.str(), FOREGROUND_GREEN | FOREGROUND_RED };
	vPlayers->push_back(goPlayer);
}

void CreateUI(HANDLE hConsole, vector<GameObject>* vUI) {
	int consoleWidth = GetConsoleWidth(hConsole);
	// create heading up the top
	wstringstream UIHeader;
	UIHeader << setw(consoleWidth) << " " << endl 
			 << setw(consoleWidth) << left << " Super Running Man vs Zombies!" << endl 
			 << setw(consoleWidth) << " ";

	GameObject goUIHeader = {0, 0, 0, 0, UIHeader.str(), BACKGROUND_BLUE | FOREGROUND_INTENSITY};
	vUI->push_back(goUIHeader);

	// create score placeholder
	// the score can be at a fixed position in the UI vector (lazy xD)
	int UIScoreWidth = 20;
	wstringstream scoreString;
	scoreString << "";
	GameObject goUIScore = {(consoleWidth - UIScoreWidth), 0, 0, 0, scoreString.str(), BACKGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED};
	vUI->push_back(goUIScore);

	// create health placeholder
	int UIHealthWidth = 42;
	wstringstream healthString;
	healthString << "";
	GameObject goUIHealthBlocks = {(consoleWidth - UIHealthWidth), 1, 0, 0, healthString.str(), BACKGROUND_RED | FOREGROUND_INTENSITY};
	vUI->push_back(goUIHealthBlocks);

	GameObject goUIHealth = {(consoleWidth - UIHealthWidth), 2, 0, 0, healthString.str(), BACKGROUND_BLUE | FOREGROUND_INTENSITY};
	vUI->push_back(goUIHealth);
}

// creates a certain amount of enemies at random locations on the screen
void CreateEnemies(HANDLE hConsole, vector<GameObject>* vEnemies, Boundry boundry, int amount) {
	wstringstream enemyString;
	enemyString << wchar_t(0x263A); // weird face
	int maxEnemies = (boundry.bottom * boundry.right);
	if(amount > maxEnemies) amount = maxEnemies;
	for(int i = 0; i < amount; i++) {
		int posX, posY = 0;
		// check for a fixed position
		if((boundry.right - boundry.left) == 0) {
			posX = boundry.left;
		} else {
			posX = rand() % boundry.right + boundry.left;
		}

		if((boundry.bottom - boundry.top) == 0) {
			posY = boundry.top;
		} else {
			posY = rand() % boundry.bottom + boundry.top; // offset for top of header
		}

		bool addEnemy = true;
		for(unsigned int e = 0; e < vEnemies->size(); e++) {
			if(vEnemies->at(e).x == posX && vEnemies->at(e).y == posY) {
				addEnemy = false;
				break;
			}
		}
		if(addEnemy) {
			GameObject newEnemy = { posX, posY, 0, 1, enemyString.str(), FOREGROUND_GREEN };
			vEnemies->push_back(newEnemy);
		} else {
			i--; // try again
		}
	}
}

void MoveEnemiesUp(vector<GameObject>* vEnemies, int amount) {
	for(unsigned int i = 0; i < vEnemies->size(); i++) {
		vEnemies->at(i).y -= amount;
	}
}

// function to shoot, creates a projectile then adds it to the projectiles vector
void Shoot(vector<GameObject>* vProjectiles, GameObject player, Score* scoreTracker, MouseState mouseState) {
	// dual guns :D
	wstringstream projectileString;
	projectileString << ".";
	if(mouseState.leftButton) {
		GameObject projectile_left = { player.x - 1, player.y, 0, 1, projectileString.str(), FOREGROUND_RED };
		vProjectiles->push_back(projectile_left);
		scoreTracker->shotsFired++;
	}

	if(mouseState.rightButton) {
		GameObject projectile_right = { player.x + 1, player.y, 0, 1, projectileString.str(), FOREGROUND_RED };
		vProjectiles->push_back(projectile_right);
		scoreTracker->shotsFired++;
	}	
}

// function to update the score on the UI, takes in the current score and formats it
void UpdateScore(vector<GameObject>* vUI, Score score) {
	int UIScoreWidth = 20;
	wstringstream UIScore, UIHealthBlocks, UIHealth; // makes formatting easier xD

	// calculate the accuracy of the shots
	double accuracy = 0.0f;
	if(score.enemiesKilled != 0 && score.shotsFired != 0) {
		accuracy = ((double)score.enemiesKilled / (double)score.shotsFired) * 100.0f;
	}
	// accuracy shouldn't be over 100%
	if(accuracy > 100.0f) accuracy = 100.0f;

	UIScore << "Killed:      " << setprecision(4) << score.enemiesKilled << endl
			<< "Shots Fired: " << setprecision(4) << score.shotsFired << endl
			<< "Accuracy:    " << setprecision(3) << accuracy << "%";

	vUI->at(1).text = UIScore.str();

	int numHealthBlocks = (int)floor(score.health / 5.0f);
	UIHealthBlocks << setw(numHealthBlocks) << "";
	vUI->at(2).text = UIHealthBlocks.str();

	UIHealth << wchar_t(9829) << " " << setprecision(3) << score.health;
	vUI->at(3).text = UIHealth.str();
}

// update the positions of the projectiles
void UpdateProjectiles(vector<GameObject>* vProjectiles) {
	for(unsigned int i = 0; i < vProjectiles->size(); i++) {
		// if its off the screen, remove it
		// otherwise decrease the Y (move it up)
		if(vProjectiles->at(i).y == 1) 
			vProjectiles->erase(vProjectiles->begin() + i);
		else
			vProjectiles->at(i).y--;
	}
}

void UpdateEnemies(HANDLE hConsole, vector<GameObject>* vEnemies, WaveConfig* waveConfig, Boundry movementBoundry, GameObject player) {

	// gravitate to player method
	for(unsigned int i = 0; i < vEnemies->size(); i++) {
		// dodgy way of doing movement timing
		int randAction = rand() % waveConfig->enemyMovementSpeed;
		//randAction = 1;

		// crude sorta search method to move towards the player and find a free space at random
		// (they are zombies after all, no idea what theyre doing ;P)
		if(randAction == 1) {
			// get the current position
			int curY = vEnemies->at(i).y;
			int curX = vEnemies->at(i).x;

			// update positions to move closer to the player
			if(player.y > curY) 
				curY++; 
			else
				curY--;

			if(player.x > curX) 
				curX++; 
			else
				curX--;

			bool updatePos = true;
			// check our new position for collisions with another enemy
			// if there is, add some random numbers to the position and try again.
			for(unsigned int e = 0; e < vEnemies->size(); e++) {
				if(vEnemies->at(e).y == curY && vEnemies->at(e).x == curX) {
					//curY += rand() % 20;
					curX += rand() % 20;
					updatePos = false;
					if(e != 0) e--; // try again
				}
			}

			// check we are inside the boundry..
			// if not, dont bother
			if(curX >= movementBoundry.left 
				&& curX <= movementBoundry.right 
				&& curY >= movementBoundry.top 
				&& curY <= movementBoundry.bottom) {
				// finally, update the position of the enemy
				if(updatePos) {
					vEnemies->at(i).y = curY;
					vEnemies->at(i).x = curX;
				}
			}
		}
	}

	// derp around method
	/*
	for(unsigned int i = 0; i < vEnemies->size(); i++) {
		// random direction to move
		int randAction = rand() % 100;
		if(randAction == 0) // move Y up
			if((vEnemies->at(i).y - 1) >= 4) vEnemies->at(i).y--;

		if(randAction == 1) // move Y down
			if((vEnemies->at(i).y + 1) <= (height - 3)) vEnemies->at(i).y++;

		if(randAction == 2) // move X left
			if((vEnemies->at(i).x - 1) >= 1) vEnemies->at(i).x--;

		if(randAction == 3) // move X right
			if((vEnemies->at(i).x + 1) <= (width - 2)) vEnemies->at(i).x++;
	}
	*/
}

// demented animation function
// loops over each animation and checks its step
// adds the current step of the animation to the effects vector for rendering :)
void UpdateAnimations(vector<Animation>* vAnimations, vector<GameObject>* vEffects) {
	// clear current effects
	vEffects->clear();
	// for every animation
	for(unsigned int a = 0; a < vAnimations->size(); a++) {
		// put the current step of the animation into the effects vector
		vEffects->push_back(vAnimations->at(a).steps[vAnimations->at(a).currentStep]);
		// increment the step
		vAnimations->at(a).currentStep++;
		// find total number of steps
		int numsteps = vAnimations->at(a).steps.size();
		// if we have reached the end of the animation...
		if(vAnimations->at(a).currentStep == numsteps) {
			if(vAnimations->at(a).loop) // if we are looping, reset the current step to 0
				vAnimations->at(a).currentStep = 0;
			else // otherwise remove the animation
				vAnimations->erase(vAnimations->begin() + a);
		}
	}
}

// checks for a collision with a projectile and an enemy, if it detects one, both are removed,
// and explosion effect is created and the score is updated.
void CheckKills(HANDLE hConsole, vector<GameObject>* vProjectiles, vector<GameObject>* vEnemies, vector<Animation>* vAnimations, Score* scoreTracker) {
	for(unsigned int i = 0; i < vProjectiles->size(); i++) {
		for(unsigned int e = 0; e < vEnemies->size(); e++) {
			if(CheckCollision(vEnemies->at(e), vProjectiles->at(i))) {
				// create explosion animation
				CreateExplosion(vAnimations, vEnemies->at(e).x, vEnemies->at(e).y);
				// remove enemy
				vEnemies->erase(vEnemies->begin() + e);
				// also remove projectile
				vProjectiles->erase(vProjectiles->begin() + i);
				// update score
				scoreTracker->enemiesKilled++;
				// add a new enemy at random :D
				//CreateEnemies(hConsole, vEnemies, 1);
				break; // we is done here
			}
		}
	}
}

void CheckPlayerHit(vector<GameObject>* vPlayers, vector<GameObject>* vEnemies, Score* scoreTracker, WaveConfig* waveConfig) {
	for(unsigned int i = 0; i < vPlayers->size(); i++) {
		for(unsigned int e = 0; e < vEnemies->size(); e++) {
			if(CheckCollision(vEnemies->at(e), vPlayers->at(i))) {
				int chanceHit = rand() % waveConfig->hitChance;
				if(chanceHit == 1) {
					if(scoreTracker->health != 0.0f)
						scoreTracker->health -= waveConfig->damageRate;
				}
				break; // we is done here
			}
		}
	}
}

// checks for a collision between two game objects
bool CheckCollision(GameObject first, GameObject second) {
	// super basic collision detection for now...
	if(second.x == first.x && second.y == first.y) return true;
	return false;
}

void CreateExplosion(vector<Animation>* vAnimations, int x, int y) {
	Animation explosion;
	wstringstream ex_step1, ex_step2, ex_step3;
	// anim step 1 looks sorta like
	//  # 
	// ###
	//  #
	ex_step1 << TP << wchar_t(0x263C) << TP << endl
			 << wchar_t(0x263C) << char(176) << wchar_t(0x263C) << endl
			 << TP << wchar_t(0x263C) << TP;

	// anim step 2 looks sorta like
	//  x#x 
	// x###x
	//  x#x
	ex_step2 << TP << 'x' << wchar_t(0x263C) << 'x' << TP << endl
			 << 'x' << wchar_t(0x263C) << char(176) << wchar_t(0x263C) << 'x' << endl
			 << TP << 'x' << wchar_t(0x263C) << 'x' << TP;

	// anim step 3 looks sorta like
	//  # 
	// ###
	//  #
	ex_step3 << TP << wchar_t(0x263C) << TP << endl
			 << wchar_t(0x263C) << char(176) << wchar_t(0x263C) << endl
			 << TP << wchar_t(0x263C) << TP;

	GameObject go_step1 = {x, y, 0, 1, ex_step1.str(), FOREGROUND_RED | FOREGROUND_BLUE}; // violet
	GameObject go_step2 = {x, y, 0, 1, ex_step2.str(), FOREGROUND_RED}; // red
	GameObject go_step3 = {x, y, 0, 1, ex_step3.str(), FOREGROUND_GREEN | FOREGROUND_RED}; // yelow

	// lazy way to make it happen twice 
	explosion.steps.push_back(go_step1);
	explosion.steps.push_back(go_step2);
	explosion.steps.push_back(go_step3);

	explosion.steps.push_back(go_step1);
	explosion.steps.push_back(go_step2);
	explosion.steps.push_back(go_step3);

	explosion.currentStep = 0;
	explosion.loop = false;

	vAnimations->push_back(explosion);
}

void ProcessInput(HANDLE hInput, vector<GameObject>* vPlayers, vector<GameObject>* vProjectiles, MouseState* mouseState, Score* scoreTracker) {
	DWORD events_read;
	INPUT_RECORD last_events[128];

	// check for events...
	PeekConsoleInput(hInput, last_events, 128, &events_read);
	// if there are some events lets process them
	if(events_read > 0)
	{
		// read the actual events so they arent in the buffer anymore
		ReadConsoleInput(hInput, last_events, 128, &events_read);

		// loop over each event
		for(unsigned int i = 0; i < events_read; i++)
		{
			// if a key event
			if(last_events[i].EventType == KEY_EVENT) {
				switch(last_events[i].Event.KeyEvent.uChar.AsciiChar)
				{
				case char(27): // esc key
					scoreTracker->gameStarted = false; // stop game
					break;
				}
			}

			// if mouse event
			if(last_events[i].EventType == MOUSE_EVENT) {
				// reset mouse state
				mouseState->buttonDown = false;
				mouseState->leftButton = false;
				mouseState->rightButton = false;

				// Update player position with mouse position
				vPlayers->at(0).x = last_events[i].Event.MouseEvent.dwMousePosition.X;
				vPlayers->at(0).y = last_events[i].Event.MouseEvent.dwMousePosition.Y;

				// if a button is pressed in this event, mark the mouse button as pressed
				if(last_events[i].Event.MouseEvent.dwButtonState) 
					mouseState->buttonDown = true;

				// if the left button is pressed
				if(last_events[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
					mouseState->leftButton = true;

				// if the right button is pressed
				if(last_events[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) 
					mouseState->rightButton = true;


				// if both are pressed at once
				if(last_events[i].Event.MouseEvent.dwButtonState == (FROM_LEFT_1ST_BUTTON_PRESSED | RIGHTMOST_BUTTON_PRESSED)) {
					mouseState->rightButton = true;
					mouseState->leftButton = true;
				} 
			}
		}
	}
	// if we have a mouse button down, shoot :D
	if(mouseState->buttonDown) Shoot(vProjectiles, vPlayers->at(0), scoreTracker, *mouseState);
}
