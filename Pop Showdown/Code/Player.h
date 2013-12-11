#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "common.h"
#include "MovableEntity.h"

class Player : public MovableEntity {
private:
	float m_Health;
	vector<Item*> m_Items;
	Item* m_ItemInHand;
public:
	Player() : MovableEntity() {
		m_Health = 100.0f;
	}

	void AddItem(Item* item) {
		m_Items.push_back(item);
	}

	void SetHandItem(Item* item) { 
		RemoveEnt(m_ItemInHand);
		m_ItemInHand = item;
		AttachEnt(item);
	}

	void Hurt(float amt) { 
		m_Health -= amt;
		if(m_Health <= 0.0f) {
			m_Alive = false;
			SetSprite(m_DeathSprite);
		}
	}

	void AddHealth(float amt) { m_Health += amt; }

	Item* HandItem() { return m_ItemInHand; }

	float Health() { return m_Health; }

};

#endif