

#ifndef ENTITY_H
#define ENTITY_H

#include "platform.h"
#include "vec2.h"

enum EntityType
{
	Entity_None,
	Entity_Player,
	Entity_Enemy,
	Entity_Pillar,
	Entity_Wall,
	Entity_Door,
};

struct Entity
{
	vec2 position;
	vec2 dims;
	vec2 forward;
	vec2 velocity;
	r32 rotation;
	r32 speed;
	u32 TexIndex;
	bool isAlive;
	EntityType type;
};

class EntityManager
{
public:
	EntityManager() { }
	~EntityManager() { }

	internal_ u32 AddEntity(EntityType type)
	{
		Assert((entityCount + 1 < ArrayCount(entities)));
		u32 EntityIndex = entityCount++;

		Entity* entity = entities + EntityIndex;
		entity->type = type;
		entity->velocity = {};

		return EntityIndex;
	}

	internal_ Entity* GetEntity(u32 EntityIndex)
	{
		Assert((EntityIndex < entityCount));
		Entity* result = entities + EntityIndex;
		return result;
	}

	internal_ void RemoveEntity(u32 EntityIndex)
	{
		Assert((EntityIndex < entityCount));
		Entity* result = entities + EntityIndex;
		result = {};
		result->type = Entity_None;
	}

	internal_ u32 EntityCount()
	{
		return entityCount;
	}

protected:
	global_variable u32 entityCount;
	global_variable Entity entities[4096];
};

#endif
