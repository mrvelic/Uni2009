#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "common.h"
#include "MovableEntity.h"

class Enemy : public MovableEntity {
private:
	float m_Health;
	float m_AggroDistance;
	float m_MovementSpeed;
	bool m_Angry;
	bool m_HasItemInHand;
	int m_IQ;
	Item* m_WeaponInHand;
public:
	// Default constructor
	Enemy() : MovableEntity() {
		m_Health = 100.0f;
		m_AggroDistance = 200.0f;
		m_MovementSpeed = 30.0f;
		m_HasItemInHand = false;
		m_Angry = false;
	}

	// Hurt the enemy
	void Hurt(float amt) { 
		m_Health -= amt; 
		if(m_Health <= 0.0f) {
			m_Alive = false;
			SetSprite(m_DeathSprite);
		}
	}

	// Properties
	void Angry(bool a) { m_Angry = a; }
	bool Angry() { return m_Angry; }
	void Health(float hp) { m_Health = hp; }
	float Health() { return m_Health; }
	void MovementSpeed(float ms) { m_MovementSpeed = ms; }
	float MovementSpeed() { return m_MovementSpeed; }
	void AggroDistance(float ad) { m_AggroDistance = ad; }
	float AggroDistance() { return m_AggroDistance; }
	int IQ() { return m_IQ; }
	void IQ(int iq) { m_IQ = iq; }

	// Set the item in hand
	void SetHandItem(Item* item) { 
		RemoveEnt(m_WeaponInHand);
		m_WeaponInHand = item;
		m_HasItemInHand = true;
		AttachEnt(item);
	}

	// Stuff for checking the item in the hand
	Item* HandItem() { return m_WeaponInHand; }
	bool ItemInHand() { return m_HasItemInHand; }

	// Reads in an enemy from a file, the file HAS to be in the right format or the world will explode
	void LoadFromFile(string filename, vector<Item*>* globalItems) {
		string line;
		ifstream file(filename.c_str());
		if(file.is_open()) {
			while(getline(file, line)) {
				if(line[0] != '#') {
					vector<string> data = Tools::SplitString(line, ',');
					string ident = data[0];
				
					if(ident == "enemy") {
						Sprite* enemySprite = new Sprite(data[1]);
						Sprite* deathSprite = new Sprite(data[2]);
						this->SetSprite(enemySprite);
						this->NormalSprite(enemySprite);
						this->DeathSprite(deathSprite);
						this->X((float)atof(data[3].c_str()));
						this->Y((float)atof(data[4].c_str()));
						this->Z((float)atof(data[5].c_str()));
						this->MovementSpeed((float)atof(data[6].c_str()));
						this->AggroDistance((float)atof(data[7].c_str()));
						enemySprite->Scale((float)atof(data[8].c_str()));
						deathSprite->Scale((float)atof(data[8].c_str()));
						this->IQ(atoi(data[9].c_str()));
						m_Health = (float)atof(data[10].c_str());
					}

					if(ident == "item") {
						for(unsigned int i = 0; i < globalItems->size(); i++) {
							if(globalItems->at(i)->Name() == data[1]) {
								Item* newItem = new Item(globalItems->at(i));
								this->SetHandItem(newItem);
							}
						}
					}
				}
			}
			file.close();
		}
	}
};

#endif