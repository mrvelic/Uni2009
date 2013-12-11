#ifndef _ITEM_H_
#define _ITEM_H_
#include "common.h"
#include "MovableEntity.h"

class Projectile : public MovableEntity {
private:
	float m_Speed;
	float m_Damage;
public:
	Projectile() : MovableEntity() {
		m_Speed = 200.0f;
	}

	Projectile(float speed, float damage) : MovableEntity() {
		m_Speed = speed;
		m_Damage = damage;
	}

	Projectile(Projectile* proj) : MovableEntity() {
		m_Speed = proj->Speed();
		m_Damage = proj->Damage();
		m_LevelBounds = proj->GetBounds();
		SetSprite(proj->GetSprite());
	}

	float Speed() { return m_Speed; }
	float Damage() { return m_Damage; }
	void Damage(float dmg) { m_Damage = dmg; }

	virtual void Update() {
		double currentTime = Timer::GetClockDouble();
		double timeSpan = currentTime - m_LastUpdate;

		if(m_LastUpdate != 0.0f) {
			if(m_Direction == 1) 
				m_X -= m_Speed * (float)timeSpan;
			else
				m_X += m_Speed * (float)timeSpan;
		}

		m_LastUpdate = currentTime;
	}
};

class Item : public Entity {
public:
	float AddHealth, BulletYOffset;
	Projectile* WeaponProjectile;
	bool IsWeapon, Disposable;
	double rotationUpdate;
	string m_Name;

	Item() : Entity() {
		AddHealth = 0.0f;
		IsWeapon = false;
		Disposable = true;
		BulletYOffset = 0.0f;
		m_Name = "Item";
	}
	Item(float health) : Entity() {
		AddHealth = health;
		Disposable = true;
		IsWeapon = false;
	}

	virtual void Update() {
		Entity::Update();
	}

	Item(Item* item) : Entity() {
		AddHealth = item->AddHealth;
		IsWeapon = item->IsWeapon;
		Disposable = item->Disposable;
		BulletYOffset = item->BulletYOffset;
		m_Name = item->Name();
		SetSprite(item->GetSprite());
		m_Roll = 0.0f;
		m_X = item->X();
		m_Y = item->Y();
		WeaponProjectile = item->WeaponProjectile;
	}

	void UpdateRotation() {
	}

	string Name() { return m_Name; }
	void Name(string name) { m_Name = name; }

	~Item() {
		delete WeaponProjectile;
	}
};

class Weapon : public Item {
public:
	Weapon() : Item() { IsWeapon = true; }
	Weapon(Projectile* proj) : Item() {
		WeaponProjectile = proj;
		IsWeapon = true;
		Disposable = false;
		BulletYOffset = 0.0f;
	}

	virtual void Update() {
		Item::Update();
	}
};




#endif