/*******************************************************
FileName: Raycast.h
Project Name: Doom Game
StartDate: 01/20/2018
ModDate: 02/27/2018
CreatorName: George Wulfers
ContributorsName: George Wulfers
===================================================
All of the code for the game will be held here within this class.
*******************************************************/

#ifndef RAYCAST_H
#define RAYCAST_H

#include "platform.h"
#include "Entity.h"

struct RaycastHitResult
{
	GameObject* entity;
	vec2 point;
	r32 distance;
	u32 entityIndex;
};

internal_ bool Raycast(vec2 pos, vec2 dir, RaycastHitResult* HitResult, r32 maxDistance = 16.0f, u32 IgnoreTag = 0)
{
	// Ray cast
	r32 stepSize = 0.1f;
	while (!HitResult->entity && HitResult->distance < maxDistance)
	{
		// Current distance the ray cast has traveled.
		HitResult->distance += stepSize;

		// current position testing for collision
		vec2 testP = { pos.x + dir.x * HitResult->distance, pos.y + dir.y * HitResult->distance };

		// Loop through our game objects to see if we have hit something.
		for (u32 entityIndex = 0; entityIndex < EntityManager::EntityCount(); ++entityIndex)
		{
			// test entity if it is not being ignored.
			GameObject* testObject = EntityManager::GetEntity(entityIndex);
			if (!testObject->CompareTag(IgnoreTag))
			{
				// Calculate the collision box.
				vec2 minP = testObject->GetPosition() - (testObject->GetDimensions() + Vec2(0.25f, 0.25f));
				vec2 maxP = testObject->GetPosition() + (testObject->GetDimensions() + Vec2(0.25f, 0.25f));

				// are we hitting this object?
				if (Intersects(testP, minP, maxP))
				{
					// Collision
					HitResult->entity = testObject;
					HitResult->point = testP;
					HitResult->entityIndex = entityIndex;

					return true;
				}
			}
		}
	}

	// No Collision
	return false;
}

#endif