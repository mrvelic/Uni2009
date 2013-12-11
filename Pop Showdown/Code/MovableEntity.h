#ifndef _MOVABLEENTITY_H_
#define _MOVABLEENTITY_H_
#include "common.h"
#include "Entity.h"
#include "DerpPhysics.h"
#include "Game.h"
#include "Timer.h"
#include "LevelBoundries.h"

class MovableEntity : public Entity, public Physics {
protected:
	bool m_Jumping, m_JumpInProgress, m_Floats, m_Alive, m_CheckedDead;
	float m_JumpHeight;
	float m_StartJumpY;
	LevelBoundries* m_LevelBounds;
	Sprite* m_WalkSprite;
	Sprite* m_DeathSprite;
public:

	MovableEntity() : Entity(), Physics() {
		m_Jumping = false;
		m_JumpInProgress = false;
		m_Floats = false;
		m_JumpHeight = 300.0f;
		m_StartJumpY = 0.0f;
		m_DrawFromBottom = true;
		m_Alive = true;
		m_CheckedDead = false;
	}

	void SetBounds(LevelBoundries* lb) { m_LevelBounds = lb; }
	LevelBoundries* GetBounds() { return m_LevelBounds; }

	void WalkSprite(Sprite* walk) { m_WalkSprite = walk; }
	void DeathSprite(Sprite* death) { m_DeathSprite = death; }

	Sprite* WalkSprite() { return m_WalkSprite; }
	Sprite* DeathSprite() { return m_DeathSprite; }

	bool Alive() { return m_Alive; }
	void Alive(bool a) { m_Alive = a; }
	bool CheckedDead() { return m_CheckedDead; }
	void CheckedDead(bool cd) { m_CheckedDead = cd; }

	virtual void UpdatePhysics() {
		if(!m_Alive) return;
		Vertex3f newPos;
		double currentTime = Timer::GetClockDouble();
		double timeSpan = currentTime - m_LastUpdate;

		newPos.x = m_X;
		newPos.y = m_Y;

		if(!m_JumpInProgress) {
			if(m_XVelocity < 0.0f) {
				if(abs(m_XVelocity) > m_MaxXVelocity) m_XVelocity = -m_MaxXVelocity;
			} else {
				if(m_XVelocity > m_MaxXVelocity) m_XVelocity = m_MaxXVelocity;
			}
		}
		
		if(!m_Jumping && !m_Floats) {
			m_YVelocity -= m_FallSpeed;
		}

		if(m_Jumping) {
			if(newPos.y >= (m_StartJumpY + m_JumpHeight)) m_Jumping = false;
		}

		newPos.y += (float)(m_YVelocity * timeSpan);
		newPos.x += (float)(m_XVelocity * timeSpan);

		
		// super lazy.. "collision detection"

		if(newPos.x <= m_LevelBounds->BoundryLeft) {
			newPos.x = newPos.x + (m_LevelBounds->BoundryLeft - newPos.x);
			m_XVelocity = 0.0f;
		}

		if(newPos.x >= m_LevelBounds->BoundryRight) {
			newPos.x = newPos.x + (m_LevelBounds->BoundryRight - newPos.x);
			m_XVelocity = 0.0f;
		}

		if(newPos.y <= m_LevelBounds->FloorLevel) {
			newPos.y = newPos.y + (m_LevelBounds->FloorLevel - newPos.y);
			m_Jumping = false;
			m_JumpInProgress = false;
			m_YVelocity = 0.0f;
			if(m_XVelocity <= m_Friction && m_XVelocity >= -m_Friction) m_XVelocity = 0.0f;
			if(m_XVelocity < 0.0f) m_XVelocity += m_Friction;
			if(m_XVelocity > 0.0f) m_XVelocity -= m_Friction;
		} 
		if(m_LastUpdate != 0.0f) {
			m_X = newPos.x;
			m_Y = newPos.y;
		}

		m_LastUpdate = currentTime;
	}

	void Jump() {
		m_Jumping = true;
		m_JumpInProgress = true;
		m_YVelocity += 800.0f;
		m_StartJumpY = m_Y;
	}

	bool IsJumping() { return m_JumpInProgress; }

	virtual void Update() {
		UpdatePhysics();
		Entity::Update();
	}
};

#endif