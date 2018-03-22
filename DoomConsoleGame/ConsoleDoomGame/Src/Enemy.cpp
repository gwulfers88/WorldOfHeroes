#include "Enemy.h"
#include "Player.h"
#include "Raycast.h"

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
	damagedPlayer = false;
	
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
			right = Cross(Vec3(forward, 0), up);

			r32 angle = Dot(forward, target->GetForward());
			dir = Vec2(0, 0);
			if (angle <= -0.9f || angle >= 0.0f)
			{
				dir.x = 1;
			}

			RaycastHitResult HitResult = {};
			if (Raycast(position, forward, &HitResult, 2.0f, tag))
			{
				dir.x = -1;

				HitResult = {};
				if (Raycast(position, right.xy, &HitResult, 2.0f, tag))
				{
					dir.y = -1;
				}
				else
				{
					dir.y = 1;
				}
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
			forward = Normalize(target->GetPosition() - position);
			
			if (player)
			{
				r32 delay = 0.25f;
				attackTimer += deltaTime;
				if (attackTimer >= delay)
				{
					RaycastHitResult HitResult = {};
					if (Raycast(position, forward, &HitResult, 6.0f, tag))
					{
						if (HitResult.entity->CompareTag(player->GetTag()))
						{
							damagedPlayer = true;
							player->playerDamage(10);
							player->Move(Vec2(-1, 0), deltaTime);
						}
					}
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
		SetIsActive(IsAlive());
	}break;

	}
}

void Enemy::takeDamage(u32 damage)
{
	health = Maximum(0, health - damage);
	tookDamage = true;
}
