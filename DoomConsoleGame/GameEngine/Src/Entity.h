

#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"
#include "memory.h"
#include <vector>

class EntityManager
{
public:
	EntityManager() { }
	~EntityManager() { }

	internal_ void AddEntity(GameObject* gameObject)
	{
		entities.push_back(gameObject);
	}

	internal_ GameObject* GetEntity(u32 EntityIndex)
	{
		Assert((EntityIndex < entities.size()));
		GameObject* result = entities[EntityIndex];
		return result;
	}

	internal_ void RemoveEntity(u32 EntityIndex)
	{
		Assert((EntityIndex < entities.size()));
		entities.erase(entities.begin()+EntityIndex);
	}

	internal_ u32 EntityCount()
	{
		return entities.size();
	}

	internal_ void Clear()
	{
		entities.clear();
	}

protected:
	// TODO: replace vector class with our own. 
	// list of entities in the game.
	global_variable std::vector<GameObject*> entities;
};

#endif
