#ifndef _GAME_H_
#define _GAME_H_

#include "common.h"
#include "Camera.h"
#include "Entity.h"
#include "Window.h"
#include "Renderer.h"
#include "Level.h"
#include "MovableEntity.h"
#include "Player.h"

class GameState {
public:
	int EnemiesKilled;
	int ShotsFired;
	int LevelsCompleted;
	GameState() {
		EnemiesKilled = 0;
		ShotsFired = 0;
		LevelsCompleted = 0;
	}
};

class Game {
private:
	Camera* m_Camera;
	Player* m_Player;
	Level* m_ActiveLevel;
	Renderer* m_Renderer;
	Window* m_Window;
	bool m_GameRunning;
	static Game* m_Instance;
	GameState* m_GameState;
	vector<Item*> m_GameItems;
	vector<Entity*>* globalEntities;
	vector<Projectile*> m_Projectiles;
	vector<Projectile*> m_EnemyProjectiles;
	vector<GameString*> m_GameStrings;

public:
	Game() { m_GameRunning = false; }
	Game(Renderer* renderer, Window* window) { 
		m_Renderer = renderer;
		m_Window = window;
		m_GameRunning = false;
		m_GameState = new GameState();
	}
	void SetInstance(Game* instance) { m_Instance = instance; }
	Level* GetActiveLevel() { return m_ActiveLevel; }
	static Game* Instance();
	GameState* Run(string filename);
	void StopGame() { m_GameRunning = false; }
	void SetupItems();
	Entity* GetPlayer() { return m_Player; }
	~Game() {
		delete m_Camera;
		delete m_Player;
	}

	void UpdateProjectiles();
	void UpdateEnemies();
	void CheckKills(Projectile* proj);
	void CheckKillsOfPlayer(Projectile* proj);
	void RemoveProjectile(Projectile* proj);
	void PlayerShoot();
	void EnemyShoot(Enemy* enemy);
	void CheckPickups();
	void DrawGameStrings();
	void SwitchLevel(string filename);
	void LoadLevel(string filename);
	void SetupPlayer();
	void CreatePlayer();
	void CreateCamera();
	void DrawLevelStrings();
	void CheckLevelEnd();
	void CheckRealKills();

	void AttachEnt(Entity* ent) { globalEntities->push_back(ent); }
	void RemoveEnt(Entity* ent) {
		for(unsigned int e = 0; e < globalEntities->size(); e++) {
			if(globalEntities->at(e) == ent) {
				globalEntities->erase(globalEntities->begin() + e);
				break;
			}
		}
	}
};



#endif
