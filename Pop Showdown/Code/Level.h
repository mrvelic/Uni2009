#ifndef _LEVEL_H_
#define _LEVEL_H_
#include "common.h"
#include "Entity.h"
#include "Terrain.h"
#include "MovableEntity.h"
#include "Item.h"
#include "Player.h"
#include "Enemy.h"
#include "random.h"
#include "LevelBoundries.h"

class Level {
private:
	vector<Enemy*> m_Enemies;
	vector<Item*> m_Items;
	vector<Entity*> m_Entities;
	vector<Entity*> m_Scenery;
	vector<Item*>* m_GlobalItems;
	vector<GameString*> m_GameStrings;

	LevelBoundries* m_Boundries;
	LevelBackground* m_Background;

	Player* m_Player;
	float m_PlayerStartX, m_PlayerStartY;
	string m_NextLevel;

	double m_LastUpdate;

public:
	// Default level constructor
	Level() {
		m_Boundries = new LevelBoundries();
		m_Boundries->FloorLevel = -200.0f;
		m_NextLevel = "none";
	}

	// Reads in a level from a file, must be in a specific format! (or bad things will happen)
	void LoadFromFile(string filename) {
		string texBoundryGrass, texBoundrySide, texBoundryGround;
		float boundryLeft, boundryRight, floorLevel;
		Colour* bgTop, *bgBottom;

		string line;
		ifstream file(filename.c_str());
		if(file.is_open()) {
			while(getline(file, line)) {
				if(line[0] != '#') {
					vector<string> data = Tools::SplitString(line, ',');
					string ident = data[0];

					if(ident == "player_start") {
						m_PlayerStartX = (float)atof(data[1].c_str());
						m_PlayerStartY = (float)atof(data[2].c_str()) + 20.0f;
					}
				
					if(ident == "background_top") {
						bgTop = new Colour((float)atof(data[1].c_str()), (float)atof(data[2].c_str()), (float)atof(data[3].c_str()));
					}

					if(ident == "background_bottom") {
						bgBottom = new Colour((float)atof(data[1].c_str()), (float)atof(data[2].c_str()), (float)atof(data[3].c_str()));
					}

					if(ident == "boundries") {
						boundryLeft = (float)atof(data[1].c_str());
						boundryRight = (float)atof(data[2].c_str());
						floorLevel = (float)atof(data[3].c_str());
					}

					if(ident == "boundry_textures") {
						texBoundryGround = data[1];
						texBoundrySide = data[2];
						texBoundryGrass = data[3];
					}

					// loads an enemy from the filename
					if(ident == "enemy") {
						Enemy* fileEnemy = new Enemy();
						fileEnemy->LoadFromFile(data[1], m_GlobalItems);
						fileEnemy->X((float)atof(data[2].c_str()));
						fileEnemy->Y((float)atof(data[3].c_str()));
						AddEnemy(fileEnemy);
					}

					if(ident == "item") {
						Item* fileItem = new Item(FindItem(data[1]));
						fileItem->X((float)atof(data[2].c_str()));
						fileItem->Y((float)atof(data[3].c_str()));
						AddItem(fileItem);
					}

					if(ident == "gamestring") {
						GameString* gs = new GameString();
						gs->Text = data[1];
						gs->TextColour = Colour(255,255,255);
						gs->X = (float)atof(data[2].c_str());
						gs->Y = (float)atof(data[3].c_str());
						m_GameStrings.push_back(gs);
					}

					if(ident == "endpoint") {
						m_Boundries->ExitPoint = (float)atof(data[1].c_str());
						m_NextLevel = data[2];
					}
				}
			}
			file.close();

			m_Background = new LevelBackground(bgTop, bgBottom);
			m_Boundries->BoundryLeft = boundryLeft;
			m_Boundries->BoundryRight = boundryRight;
			m_Boundries->FloorLevel = floorLevel;
			CreateBoundries(texBoundryGround, texBoundrySide, texBoundryGrass);
		}
	}

	string NextLevel() {
		return m_NextLevel;
	}

	// Needed for finding items and what not
	void SetGlobalItems(vector<Item*>* items) {
		m_GlobalItems = items;
	}

	// sets player at starting pos
	void SetStartPos() {
		m_Player->X(m_PlayerStartX);
		m_Player->Y(m_PlayerStartY);
	}

	// Returns the level boundries (handy for "collision" detection)
	LevelBoundries* GetBoundries() { return m_Boundries; }

	// creates a default background
	void SetupBackground() {
		m_Background = new LevelBackground(new Colour(64.0f, 184.0f, 255.0f), new Colour(0, 0, 200));
	}

	// gets the pointer to the background
	LevelBackground* GetBackground() {
		return m_Background;
	}

	// gets a pointer to the list of enemies
	vector<Enemy*>* GetEnemies() { return &m_Enemies; }

	// creates the boundries on the level
	void CreateBoundries(string groundTexture, string sideTexture, string grassTexture) {
		float farLeft = m_Boundries->BoundryLeft - 2000;
		float farRight = m_Boundries->BoundryRight + 2000;

		Sprite* dirtSprite = new Sprite(groundTexture);
		dirtSprite->Scale(256.0f);

		Sprite* grassSprite = new Sprite(grassTexture);
		grassSprite->Scale(256.0f);

		Sprite* sideDirtSprite = new Sprite(sideTexture);
		sideDirtSprite->Scale(256.0f);

		Sprite* flagSprite = new Sprite("flag.png");
		flagSprite->Scale(500.0f);

		Entity* flagEnt = new Entity();
		flagEnt->SetSprite(flagSprite);
		flagEnt->X(m_Boundries->ExitPoint);
		flagEnt->Y(m_Boundries->FloorLevel);
		flagEnt->DrawFromBottom(true);
		m_Entities.push_back(flagEnt);

		float floorOffset = m_Boundries->FloorLevel - (dirtSprite->Height() / 2);

		for(float newFloor = floorOffset; newFloor > -2000.0f; newFloor -= dirtSprite->Height()) {
			for(float x = farLeft; x < farRight; x+= dirtSprite->Width()) {
				Entity* terrEnt = new Entity();
				terrEnt->SetSprite(dirtSprite);
				terrEnt->X(x);
				terrEnt->Y(newFloor);
				m_Entities.push_back(terrEnt);
			}
		}

		for(float x = farLeft; x < farRight; x+= grassSprite->Width()) {
			Entity* terrEnt = new Entity();
			terrEnt->SetSprite(grassSprite);
			terrEnt->X(x);
			terrEnt->Y(floorOffset + grassSprite->Height() / 2);
			m_Entities.push_back(terrEnt);
		}

		for(float sideWidth = 0.0f; sideWidth < 1500.0f; sideWidth += sideDirtSprite->Width()) {

			float newXLeft = m_Boundries->BoundryLeft - (sideDirtSprite->Width() / 2) - sideWidth;
			float newXRight = m_Boundries->BoundryRight + (sideDirtSprite->Width() / 2) + sideWidth;

			for(float y = m_Boundries->FloorLevel - sideDirtSprite->Height(); y < 2000; y+= sideDirtSprite->Height()) {
				Entity* terrEnt = new Entity();
				terrEnt->SetSprite(sideDirtSprite);
				terrEnt->X(newXLeft);
				terrEnt->Y(y);
				m_Entities.push_back(terrEnt);

				terrEnt = new Entity();
				terrEnt->SetSprite(sideDirtSprite);
				terrEnt->X(newXRight);
				terrEnt->Y(y);
				m_Entities.push_back(terrEnt);
			}

		}
	}

	// loads a default test level
	void LoadLevel() {
		m_Boundries->BoundryLeft = -400.0f;
		m_Boundries->BoundryRight = 10000.0f;

		m_Boundries->LevelSpan = (m_Boundries->BoundryRight + 2000) - (m_Boundries->BoundryLeft - 2000);

		CreateBoundries("ground.png", "side_dirt.png", "grass.png");

		SetupBackground();
		Enemy* test = new Enemy();
		Sprite* newSprite = new Sprite("lolkesha.png");
		Sprite* newSprite2 = new Sprite("test.png");
		test->SetSprite(newSprite);
		test->DeathSprite(newSprite2);
		test->X(2000.0f);
		test->Y(320.0f);
		test->Z(0.000f);
		test->MovementSpeed(20.0f);
		test->AggroDistance(1000.0f);
		newSprite->Scale(500.0f);
		newSprite2->Scale(500.0f);
		test->SetHandItem(FindItem("MachineGun"));
		test->IQ(190);

		//AddEnemy(test);


		Enemy* testFile = new Enemy();
		testFile->LoadFromFile("testenemy.txt", m_GlobalItems);
		AddEnemy(testFile);

		Item* health = new Item(FindItem("HealthPickup"));
		health->X(2000.0f);
		health->Y(-100.0f);
		AddItem(health);
	}

	// finds an item based on the name in the global items list
	Item* FindItem(string name) {
		for(unsigned int i = 0; i < m_GlobalItems->size(); i++) {
			if(m_GlobalItems->at(i)->Name() == name) {
				return m_GlobalItems->at(i);
			}
		}

		return NULL;
	}

	// adds an enemy to the level
	void AddEnemy(Enemy* enemy) {
		m_Enemies.push_back(enemy);
		enemy->SetBounds(m_Boundries);
		RegisterEntity(enemy);
	}

	// adds a pickup item to the level
	void AddItem(Item* item) {
		m_Items.push_back(item);
		RegisterEntity(item);
	}

	// removes an item from the level
	void RemoveItem(Item* item) {
		for(unsigned int i = 0; i < m_Items.size(); i++) {
			if(m_Items[i] == item) {
				m_Items.erase(m_Items.begin() + i );
			}
		}
	}

	// gets a pointer to the list of items on this level
	vector<Item*>* GetItems() {
		return &m_Items;
	}

	// register entity with global game entity list
	void RegisterEntity(Entity* ent) {
		m_Entities.push_back(ent);
	}

	// sets the player
	void SetPlayer(Player* player) {
		m_Player = player;
	}

	// updates certain animations etc for the level
	void UpdateLevel() {
		for(unsigned int i = 0; i < m_Items.size(); i++) {
			Item* ci = m_Items[i];
			double currentTime = Timer::GetClockDouble();
			double timeSpan = currentTime - ci->rotationUpdate;

			ci->IgnoreDirection(true);

			if(ci->rotationUpdate != 0.0f) {
					ci->Roll((float)(ci->Roll() + (50.0f * timeSpan)));
					if(ci->Roll() >= 360.0f) { ci->Roll(0.0f); }
			}

			ci->rotationUpdate = currentTime;
		}
	}

	// gets the list of overall entities
	vector<Entity*>* GetEntities() { return &m_Entities; }

	// get game strings
	vector<GameString*>* GetGameStrings() { return &m_GameStrings; }

	// deconstructor
	~Level() {
		delete m_Boundries;
	}
};

#endif