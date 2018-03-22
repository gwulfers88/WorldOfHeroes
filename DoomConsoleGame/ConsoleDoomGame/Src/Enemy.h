/*******************************************************
FileName: DemoGame.h
Project Name: Doom Game
StartDate: 01/20/2018
ModDate: 02/27/2018
CreatorName: George Wulfers
ContributorsName: George Wulfers
===================================================
All of the code for the game will be held here within this class.
*******************************************************/


#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"

enum EnemyState
{
	ES_Idle,
	ES_FollowTarget,
	ES_Attack,
	ES_Die,
};

class Enemy : public GameObject
{
public:
	Enemy();
	virtual ~Enemy();

	void setMaxHealth() { maxHealth = 100; }
	void setHealth(u32 amount);
	void setTarget(GameObject* newTarget) { target = newTarget; }
	void setTookDamage(bool damaged) { tookDamage = damaged; }

	u32 getHealth() { return health; }
	u32 getDamage() { return damage; }
	bool getTookDamage() { return tookDamage; }

	virtual void Update(r32) override;
	bool IsAlive() { return getHealth() > 0; }
	void takeDamage(u32 damage);
	bool hasDamagedPlayer() { return damagedPlayer; }

protected:
	bool damagedPlayer;
	bool tookDamage;

	u32 health;
	u32 maxHealth;
	u32 damage;

	GameObject* target;
	EnemyState currentState;
	r32 attackTimer;
};

#endif
