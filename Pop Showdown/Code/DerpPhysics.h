#ifndef _PHYSICS_H_
#define _PHYSICS_H_

class Physics {
protected:
	double m_LastUpdate;
	float m_XVelocity;
	float m_YVelocity;
	float m_Mass;
	float m_Friction;
	float m_MaxVelocity;
	float m_FallSpeed;
	float m_MaxXVelocity;
	
public:
	Physics() {
		m_LastUpdate = 0;
		m_XVelocity = 0.0f;
		m_YVelocity = 0.0f;
		m_Mass = 0.0f;
		m_Friction = 15.0f;
		m_MaxVelocity = 800.0f;
		m_FallSpeed = 100.0f;
		m_MaxXVelocity = 700.0f;
	}

	void AddVelocity(float xamt, float yamt) {
		m_XVelocity += xamt;
		m_YVelocity += yamt;
	}

	void AddXVelocity(float xamt) {
		m_XVelocity += xamt;
	}

	void AddYVelocity(float yamt) {
		m_YVelocity += yamt;
	}

	void SetMass(float mass) { m_Mass = mass; };
	void AddMass(float mass) { m_Mass += mass; };

	virtual void UpdatePhysics() = 0;
};


#endif