#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "platform.h"
#include "vec3.h"

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	void SetPosition(vec2);
	void SetRotation(r32);
	void SetDimensions(vec2);
	void SetSpeed(r32);
	void SetTag(u32);
	void SetCanCollide(bool);
	void SetIsActive(bool);
	void SetIsTrigger(bool);

	bool CompareTag(u32 other) { return tag == other; }

	vec2 GetPosition() { return position; }
	vec2 GetDimensions() { return dimensions; }
	vec2 GetForward() { return forward; }
	u32 GetTag() { return tag; }
	r32 GetRotation() { return rotation; }
	bool CanCollide() { return canCollide; }
	bool IsActive() { return isActive; }
	bool IsTrigger() { return isTrigger; }
	void Rotate(r32 angle) { rotation += angle; }
	void Move(vec2 direction, r32 deltaTime);

	virtual void Update(r32 deltaTime);

	vec2 dir;
protected:
	void RecalculateTransform();

	bool isDirty;
	bool canCollide;
	bool isTrigger;
	bool isActive;

	vec2 dimensions;
	vec2 position;
	vec2 velocity;
	r32 speed;
	vec2 forward;
	vec3 up;
	vec3 right;
	r32 rotation;
	r32 oldRotation;

	u32 tag;
};

#endif
