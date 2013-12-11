#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "common.h"
#include "Model.h"
#include "Sprite.h"

class Entity {
protected:
	float m_X, m_Y, m_Z, m_Scale, m_Yaw, m_Pitch, m_Roll;
	bool m_EntsAttached, m_SpriteAttached, m_ModelAttached, m_DrawFromBottom, m_IgnoreDirection;
	int m_Direction;
	vector<Entity*> m_Entities;

	Sprite* m_Sprite;
	Sprite* m_NormalSprite;
	Model* m_Model;
public:
	void X(float x) { m_X = x; }
	void Y(float y) { m_Y = y; }
	void Z(float z) { m_Z = z; }
	float X() { return m_X; }
	float Y() { return m_Y; }
	float Z() { return m_Z; }
	void Yaw(float y) { m_Yaw = y; }
	void Pitch(float p) { m_Pitch = p; }
	void Roll(float r) { m_Roll = r; }
	float Yaw() { return m_Yaw; }
	float Pitch() { return m_Pitch; }
	float Roll() { return m_Roll; }

	int Direction() { return m_Direction; }
	void Direction(int dir) { 
		for(unsigned int e = 0; e < m_Entities.size(); e++) {
			m_Entities[e]->Direction(dir);
		}
		m_Direction = dir; 
	}

	void Scale(float scale) { m_Scale = scale; }
	float Scale() { return m_Scale; }
	bool HasModel() { return m_ModelAttached; }
	bool HasSprite() { return m_SpriteAttached; }
	bool DrawFromBottom() { return m_DrawFromBottom; }
	void DrawFromBottom(bool d) { m_DrawFromBottom = d; }

	bool InCollisionRange(float x, float y) {
		float farLeft = m_X - (m_Sprite->Width() / 2) + 20.0f;
		float farRight = m_X + (m_Sprite->Width() / 2) - 20.0f;
		float top = m_Y + m_Sprite->Height();
		float bottom = m_Y - m_Sprite->Height();

		if(x < farRight && x > farLeft) {
			if(y <= top && y >= bottom) {
				return true;
			}
		}

		return false;
	}

	vector<Entity*>* GetEntities() { return &m_Entities; }
	void AttachEnt(Entity* ent) { m_Entities.push_back(ent); }
	void RemoveEnt(Entity* ent) {
		for(unsigned int e = 0; e < m_Entities.size(); e++) {
			if(m_Entities[e] == ent) {
				m_Entities.erase(m_Entities.begin() + e);
				break;
			}
		}
	}

	Entity() {
		m_X = 0.0f;
		m_Y = 0.0f;
		m_Z = 0.0f;
		m_Scale = 1.0f;
		m_Yaw = 0.0f;
		m_Pitch = 0.0f;
		m_Roll = 0.0f;
		m_Direction = 0;
		m_ModelAttached = false;
		m_SpriteAttached = false;
		m_EntsAttached = false;
		m_DrawFromBottom = false;
		m_IgnoreDirection = false;
	}

	void SetSprite(Sprite* sprite) { m_Sprite = sprite; m_SpriteAttached = true; }
	void SetModel(Model* model) { m_Model = model; m_ModelAttached = true; }

	void NormalSprite(Sprite* norm) { m_NormalSprite = norm; }
	Sprite* NormalSprite() { return m_NormalSprite; }
	
	bool IgnoreDirection() { return m_IgnoreDirection; }
	void IgnoreDirection(bool ig) { m_IgnoreDirection = ig; }

	Sprite* GetSprite() { return m_Sprite; }
	Model* GetModel() { return m_Model; }

	virtual void Update() {
		if(m_Direction == 0) {
			m_Yaw = 0.0f;
		} else {
			m_Yaw = 180.0f;
		}
	}

	virtual ~Entity() {
		if(m_SpriteAttached) delete m_Sprite;
		if(m_ModelAttached) delete m_Model;
	}
};

#endif