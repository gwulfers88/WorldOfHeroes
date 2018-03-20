#include "Enemy.h"
#include "Player.h"

Enemy::Enemy()
	: GameObject()
{
	health = 100;
	maxHealth = 100;
	damage = 10;
	target = 0;
	currentState = ES_Idle;
	attackTimer = 0;
}

Enemy::~Enemy()
{

}

void Enemy::setHealth(u32 amount)
{
	if (amount < maxHealth)
	{
		health = amount;
	}
	else
	{
		health = maxHealth;
	}
}

void Enemy::Update(r32 deltaTime)
{
	GameObject::Update(deltaTime);
	r32 distance = FLT_MAX;

	if(target)
		distance = Length(target->GetPosition() - position);

	currentState = IsAlive() ? currentState : ES_Die;

	switch (currentState)
	{
	case ES_Idle:
	{
		if (target)
		{
			if (distance >= 5.0f)
			{
				currentState = ES_FollowTarget;
			}
		}
	}break;

	case ES_FollowTarget:
	{
		if (target)
		{
			forward = Normalize(target->GetPosition() - position);
			r32 angle = Dot(forward, target->GetForward());
			dir = Vec2(0, 0);
			if (angle <= -0.9f || angle >= 0.0f)
			{
				dir.x = 1;
			}
			// Move entity
			Move(dir, deltaTime);

			if (distance >= 2.0f && distance < 5.0f)
			{
				currentState = ES_Attack;
			}
			else if(distance > 10.0f)
			{
				currentState = ES_Idle;
			}
		}
	}break;

	case ES_Attack:
	{
		if (target)
		{
			Player* player = (Player*)target;
			if (player)
			{
				r32 delay = 0.5f;
				attackTimer += deltaTime;
				if (attackTimer >= delay)
				{
					player->playerDamage(10);
					player->Move(Vec2(-1, 0), deltaTime);
					attackTimer = 0;
				}
			}

			if (distance > 5.0f)
			{
				currentState = ES_FollowTarget;
			}
			else if (distance > 10.0f)
			{
				currentState = ES_Idle;
			}
		}
	}break;

	case ES_Die:
	{
		target = 0;
	}break;

	}
}

void Enemy::takeDamage(u32 damage)
{
	health = Maximum(0, health - damage);
}
