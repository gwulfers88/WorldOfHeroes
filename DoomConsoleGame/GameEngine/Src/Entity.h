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

	internal_ GameObject* FindGameObject(u32 tag)
	{
		for (u32 index = 0; index < EntityCount(); ++index)
		{
			GameObject* gameObject = GetEntity(index);
			if (gameObject->CompareTag(tag))
			{
				return gameObject;
			}
		}
		return 0;
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
