#include "common.h"
#include "Game.h"
#include "GameInputHandler.h"
#include "InputManager.h"
#include "Player.h"

Game* Game::m_Instance = NULL;

Game* Game::Instance() {
	if(m_Instance == NULL) {
		m_Instance = new Game();
	}
	return m_Instance;
}

GameState* Game::Run(string filename) {
	m_GameRunning = true;
	
	// Load the first level
	LoadLevel(filename);

	// Create the camera, player etc
	CreateCamera();
	CreatePlayer();
	SetupPlayer();

	// Give the entities list to the renderer
	m_Renderer->SetEntities(globalEntities);

	// Setup the input handler
	InputHandler* inputHandler = new GameInputHandler(this);
	InputManager::Instance()->SetHandler(inputHandler);

	// Setup game strings
	GameString* healthString = new GameString();
	GameString* gameTitle = new GameString();

	gameTitle->Text = "Pop Showdown - Mirko Velic - 900216058";
	gameTitle->TextColour = Colour(255, 255, 255);
	gameTitle->X = -1090.0f;
	gameTitle->Y = 780.0f;

	healthString->Text = "";
	healthString->TextColour = Colour(255,255,255);
	healthString->X = -120.0f;
	healthString->Y = 0.0f;

	m_GameStrings.push_back(gameTitle);
	m_GameStrings.push_back(healthString);

	while(m_GameRunning) {
		if(GameInput::DispatchMessages()) {

			// Move player right
			if(inputHandler->GetKeyState()[0x44]) {
				m_Player->AddXVelocity(40.0f);
				m_Player->Direction(0);
				m_Player->HandItem()->Direction(0);
			}

			// Move player left
			if(inputHandler->GetKeyState()[0x41]) {
				m_Player->AddXVelocity(-40.0f);
				m_Player->Direction(1);
				m_Player->HandItem()->Direction(1);
			}

			// Stop the game
			if(inputHandler->GetKeyState()[VK_ESCAPE]) {
				m_GameRunning = false;
			}

			// Jump
			if(inputHandler->GetKeyState()[VK_SPACE]) {
				if(!m_Player->IsJumping()) {
					m_Player->Jump();
				}
			}

			// shoot
			if(inputHandler->GetKeyState()[VK_TAB]) {
				if(m_Player->HandItem()->IsWeapon) {
					PlayerShoot();
				}
			}

			// perform updates
			UpdateProjectiles();
			UpdateEnemies();
			CheckPickups();
			CheckLevelEnd();
			CheckRealKills();
			m_ActiveLevel->UpdateLevel();


			// Perform physics / animation updates on all entities :D
			for(unsigned int i = 0; i < globalEntities->size(); i++) {
				globalEntities->at(i)->Update();
			}

			// Update player stat information
			stringstream healthSS;
			healthSS << "Health: " << m_Player->Health();
			healthString->Text = healthSS.str();

			// Move camera around
			m_Camera->X(-(m_Player->X()));
			m_Camera->Y(-(m_Player->Y() + 450.0f));


			// Draw!! :D
			m_Renderer->Draw();
			DrawGameStrings();
			DrawLevelStrings();
			m_Window->WinSwapBuffers();
		}


	}

	return m_GameState;
}
// create the default camera
void Game::CreateCamera() {
	m_Camera = new Camera(0.0f, 0.0f, -2000.0f);
	m_Renderer->SetCamera(m_Camera);
}

// create the player
void Game::CreatePlayer() {
	m_Player = new Player();

	Sprite* playersprite = new Sprite("lolkesha.png");
	Sprite* playerDeathsprite = new Sprite("test.png");

	playersprite->Scale(500);

	m_Player->SetSprite(playersprite);
	m_Player->DeathSprite(playerDeathsprite);
	m_Player->X(0.0f);
	m_Player->Y(200.0f);
	m_Player->Z(0.0f);

	m_Player->AddItem(m_GameItems.at(0));
	m_Player->SetHandItem(m_GameItems.at(0));
}

void Game::CheckLevelEnd() {
	if(m_Player->X() > m_ActiveLevel->GetBoundries()->ExitPoint) {
		if(m_ActiveLevel->NextLevel() != "none") {
			SwitchLevel(m_ActiveLevel->NextLevel());
		} else {
			m_GameRunning = false;
		}
		m_GameState->LevelsCompleted++;
	}
}

// sets up the player
void Game::SetupPlayer() {
	globalEntities->push_back(m_Player);
	m_ActiveLevel->SetPlayer(m_Player);
	m_Player->SetBounds(m_ActiveLevel->GetBoundries());
	m_ActiveLevel->SetStartPos();
}

// loads a new level
void Game::LoadLevel(string filename) {
	m_ActiveLevel = new Level();
	globalEntities = m_ActiveLevel->GetEntities();
	SetupItems();
	m_ActiveLevel->SetGlobalItems(&m_GameItems);
	m_ActiveLevel->LoadFromFile(filename);
	m_Renderer->SetBackground(m_ActiveLevel->GetBackground());
}

// switches a level mid-game
void Game::SwitchLevel(string filename) {
	globalEntities->clear();
	LoadLevel(filename);
	SetupPlayer();
}

// draws UI related strings
void Game::DrawGameStrings() {
	for(unsigned int s = 0; s < m_GameStrings.size(); s++) {
		GameString* gs = m_GameStrings[s];
		m_Renderer->DrawText(-m_Camera->X() + gs->X, -m_Camera->Y() + gs->Y, gs->TextColour, gs->Text);
	}
}

// draws strings from the level
void Game::DrawLevelStrings() {
	vector<GameString*>* gstrings = m_ActiveLevel->GetGameStrings();
	for(unsigned int s = 0; s < gstrings->size(); s++) {
		GameString* gs = gstrings->at(s);
		m_Renderer->DrawText(gs->X, gs->Y, gs->TextColour, gs->Text);
	}
}

// function for player to shoot
void Game::PlayerShoot() {
	float weaponX = 0.0f, weaponY = 0.0f;
	if(m_Player->Direction() == 1) {
		weaponX = m_Player->X() - m_Player->HandItem()->X() - (m_Player->HandItem()->GetSprite()->Width() / 2);
		weaponY = m_Player->Y() + m_Player->HandItem()->Y() + m_Player->HandItem()->BulletYOffset;
	} else {
		weaponX = m_Player->X() + m_Player->HandItem()->X() + (m_Player->HandItem()->GetSprite()->Width() / 2);
		weaponY = m_Player->Y() + m_Player->HandItem()->Y() + m_Player->HandItem()->BulletYOffset;
	}
	Projectile* newProj = new Projectile(m_Player->HandItem()->WeaponProjectile);
	AttachEnt(newProj);
	m_Projectiles.push_back(newProj);
	newProj->X(weaponX);
	newProj->Y(weaponY);
	newProj->Direction(m_Player->Direction());
	m_GameState->ShotsFired++;
}

// function for enemy to shoot
void Game::EnemyShoot(Enemy* enemy) {
	float weaponX = 0.0f, weaponY = 0.0f;
	if(m_Player->Direction() == 1) {
		weaponX = enemy->X() - enemy->HandItem()->X() - (enemy->HandItem()->GetSprite()->Width() / 2);
		weaponY = enemy->Y() + enemy->HandItem()->Y() + enemy->HandItem()->BulletYOffset;
	} else {
		weaponX = enemy->X() + enemy->HandItem()->X() + (enemy->HandItem()->GetSprite()->Width() / 2);
		weaponY = enemy->Y() + enemy->HandItem()->Y() + enemy->HandItem()->BulletYOffset;
	}

	int maxIQ = 200;
	int IQval = maxIQ - enemy->IQ();

	int shootOrNot = rand() % maxIQ + IQval;
	if(shootOrNot == (maxIQ)) {
		Projectile* newProj = new Projectile(enemy->HandItem()->WeaponProjectile);
		AttachEnt(newProj);
		m_EnemyProjectiles.push_back(newProj);
		newProj->X(weaponX);
		newProj->Y(weaponY);
		newProj->Direction(enemy->Direction());
	}
}

// check enemies against a projectile
void Game::CheckKills(Projectile* proj) {
	vector<Enemy*>* ents = m_ActiveLevel->GetEnemies();
	for(unsigned int e = 0; e < ents->size(); e++) {
		Enemy* ent = ents->at(e);
		if(ent->Alive()) {
			if(ent->InCollisionRange(proj->X(), proj->Y())) {
				ent->Hurt(proj->Damage());
				RemoveProjectile(proj);
			}
		}
	}
}

// check enemies death state
void Game::CheckRealKills() {
	vector<Enemy*>* ents = m_ActiveLevel->GetEnemies();
	for(unsigned int e = 0; e < ents->size(); e++) {
		Enemy* ent = ents->at(e);
		if(!ent->Alive()) {
			if(ent->CheckedDead() == false) {
				ent->CheckedDead(true);
				m_GameState->EnemiesKilled++;
			}
		}
	}
}

// check enemy bullets against the player
void Game::CheckKillsOfPlayer(Projectile* proj) {
	if(m_Player->Alive()) {
		if(m_Player->InCollisionRange(proj->X(), proj->Y())) {
			m_Player->Hurt(proj->Damage());
			RemoveProjectile(proj);
		}
	}
}

// remove a projectile
void Game::RemoveProjectile(Projectile* proj) {
	for(unsigned int p = 0; p < m_Projectiles.size(); p++) {
		Projectile* currentP = m_Projectiles[p];
		if(currentP == proj) {
			RemoveEnt(currentP);
			m_Projectiles.erase(m_Projectiles.begin() + p);
		}
	}
}

// Update the projectiles
void Game::UpdateProjectiles() {
	for(unsigned int p = 0; p < m_Projectiles.size(); p++) {
		Projectile* currentP = m_Projectiles[p];
		if(currentP->X() > m_ActiveLevel->GetBoundries()->BoundryRight || currentP->X() < m_ActiveLevel->GetBoundries()->BoundryLeft) {
			RemoveProjectile(currentP);
		} else {
			CheckKills(currentP);
		}
	}

	for(unsigned int p = 0; p < m_EnemyProjectiles.size(); p++) {
		Projectile* currentP = m_EnemyProjectiles[p];
		if(currentP->X() > m_ActiveLevel->GetBoundries()->BoundryRight || currentP->X() < m_ActiveLevel->GetBoundries()->BoundryLeft) {
			RemoveProjectile(currentP);
		} else {
			CheckKillsOfPlayer(currentP);
		}
	}
}

// update the enemy (movement etc)
void Game::UpdateEnemies() {
	vector<Enemy*>* ents = m_ActiveLevel->GetEnemies();
	for(unsigned int e = 0; e < ents->size(); e++) {
		Enemy* ent = ents->at(e);

		if(ent->Alive()) {
			float aggroLeft = ent->X() - ent->AggroDistance();
			float aggroRight = ent->X() + ent->AggroDistance();

			if(m_Player->X() < aggroRight && m_Player->X() > aggroLeft) {
				ent->Angry(true);
			}

			if(ent->Angry()) {
				if(ent->ItemInHand()) {
					if(ent->HandItem()->IsWeapon) {
						EnemyShoot(ent);
					}
				}
				if(ent->X() < m_Player->X()) {
					ent->Direction(0);
					ent->AddXVelocity(ent->MovementSpeed());
				}

				if(ent->X() > m_Player->X()) {
					ent->Direction(1);
					ent->AddXVelocity(-ent->MovementSpeed());
				}
			}
		}
	}
}

// check for pickups
void Game::CheckPickups() {
	vector<Item*>* items = m_ActiveLevel->GetItems();
	for(unsigned int i = 0; i < items->size(); i++) {
		Item* ci = items->at(i);
		if(ci->InCollisionRange(m_Player->X(), m_Player->Y())) {
			m_Player->AddHealth(ci->AddHealth);
			items->erase(items->begin() + i);
			RemoveEnt(ci);
		}
	}
}

// create initial set of items
void Game::SetupItems() {
	m_GameItems.clear();

	Sprite* sAK47 = new Sprite("gun_ak.png");
	sAK47->Scale(256.0f);

	Sprite* sMachineGun = new Sprite("gun_machine.png");
	sMachineGun->Scale(256.0f);

	Sprite* bullet = new Sprite("test.png");
	bullet->Scale(20.0f);

	Sprite* healthSprite = new Sprite("item_health.png");
	healthSprite->Scale(100.0f);

	// AK 47 :D
	Weapon* gun_AK47 = new Weapon(new Projectile(2000.0f, 2.0f));
	gun_AK47->WeaponProjectile->SetBounds(m_ActiveLevel->GetBoundries());
	gun_AK47->WeaponProjectile->SetSprite(bullet);
	gun_AK47->SetSprite(sAK47);
	gun_AK47->X(60.0f);
	gun_AK47->Y(200.0f);
	gun_AK47->BulletYOffset = 13.0f;
	gun_AK47->Name("AK47");

	m_GameItems.push_back(gun_AK47);

	// Machine Gun :D
	Weapon* gun_Machine = new Weapon(new Projectile(2500.0f, 5.0f));
	gun_Machine->WeaponProjectile->SetBounds(m_ActiveLevel->GetBoundries());
	gun_Machine->WeaponProjectile->SetSprite(bullet);
	gun_Machine->SetSprite(sMachineGun);
	gun_Machine->X(60.0f);
	gun_Machine->Y(200.0f);
	gun_Machine->BulletYOffset = 13.0f;
	gun_Machine->Name("MachineGun");

	m_GameItems.push_back(gun_Machine);

	Item* healthPickup = new Item(30.0f);
	healthPickup->Name("HealthPickup");
	healthPickup->SetSprite(healthSprite);
	m_GameItems.push_back(healthPickup);
}