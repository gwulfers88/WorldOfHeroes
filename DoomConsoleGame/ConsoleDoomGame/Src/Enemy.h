

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

	u32 getHealth() { return health; }
	u32 getDamage() { return damage; }

	virtual void Update(r32) override;
	bool IsAlive() { return getHealth() > 0; }
	void takeDamage(u32 damage);

protected:
	u32 health;
	u32 maxHealth;
	u32 damage;

	GameObject* target;
	EnemyState currentState;
	r32 attackTimer;
};

#endif
