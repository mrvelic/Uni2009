// SIT251 Assignment 1
// Mirko Velic - 900216058

#ifndef _GAME_H_
#define _GAME_H_
#include "input.h"
const int MAX_OBJECTS = 256;

// game results
#define GR_EXITED 0
#define GR_NOHEALTH 1
#define GR_GAMEFINISHED 2

struct GameObject {
	int x, y, isDead, drawFromCentre;
	std::wstring text;
	unsigned short attributes;
};

struct Animation {
	vector<GameObject> steps;
	int currentStep;
	bool loop;
};

struct Score {
	int enemiesKilled;
	int shotsFired;
	int gameResult;
	int wave;
	double health;
	int timeLeft;
	bool gameStarted;
};

struct Boundry {
	int left;
	int top;
	int right;
	int bottom;
};

struct WaveConfig {
	int startingEnemies;
	int enemiesPerWave;
	int defaultTime;
	int timeAddedPerWave;
	int hitChance;
	int enemyMovementSpeed;
	double damageRate;
};

Score RunGame(HANDLE hConsole, HANDLE hInput);
void AppendGObjects(vector<GameObject>* vGObjects, vector<GameObject>* vAppendObjects);
void CreatePlayer(vector<GameObject>* vPlayers);
void CreateUI(HANDLE hConsole, vector<GameObject>* vUI);
void Shoot(vector<GameObject>* vProjectiles, GameObject player, Score* scoreTracker, MouseState mouseState);
void UpdateScore(vector<GameObject>* vUI, Score score);
void UpdateProjectiles(vector<GameObject>* vProjectiles);
void UpdateAnimations(vector<Animation>* vAnimations, vector<GameObject>* vEffects);
void CheckKills(HANDLE hConsole, vector<GameObject>* vProjectiles, vector<GameObject>* vEnemies, vector<Animation>* vAnimations, Score* scoreTracker);
bool CheckCollision(GameObject first, GameObject second);
void CreateExplosion(vector<Animation>* vAnimations, int x, int y);
void CreateEnemies(HANDLE hConsole, vector<GameObject>* vEnemies, Boundry boundry, int amount);
void UpdateEnemies(HANDLE hConsole, vector<GameObject>* vEnemies, WaveConfig* waveConfig, Boundry movementBoundry, GameObject player);
void ProcessInput(HANDLE hInput, vector<GameObject>* vPlayers, vector<GameObject>* vProjectiles, MouseState* mouseState, Score* scoreTracker);
void MoveEnemiesUp(vector<GameObject>* vEnemies, int amount);
void CheckPlayerHit(vector<GameObject>* vPlayers, vector<GameObject>* vEnemies, Score* scoreTracker, WaveConfig* waveConfig);
void CheckGameState(HANDLE hConsole, vector<GameObject>* vEnemies, Score* scoreTracker, WaveConfig* waveConfig, Boundry enemyBoundry);
#endif
