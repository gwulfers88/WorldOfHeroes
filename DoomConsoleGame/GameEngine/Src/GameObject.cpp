#include "GameObject.h"

GameObject::GameObject()
{
	isDirty = true;
	dimensions = {};
	position = {};
	velocity = {};
	speed = 0.0f;
	rotation = 0.0f;
	up = Vec3(0, 0, 1);
	RecalculateTransform();
}

GameObject::~GameObject()
{

}

void GameObject::SetPosition(vec2 newP)
{
	position = newP;
}

void GameObject::SetRotation(r32 newRot)
{
	rotation = newRot;
}

void GameObject::SetDimensions(vec2 newDims)
{
	dimensions = newDims;
}

void GameObject::SetSpeed(r32 newSpeed)
{
	speed = newSpeed;
}

void GameObject::SetTag(u32 newTag)
{
	tag = newTag;
}

void GameObject::Move(vec2 direction, r32 deltaTime)
{
	// Acceleration
	vec2 acceleration = ((forward * direction.x) + (right.xy * direction.y)) * speed;
	//vec2 drag = -0.8f * entity->velocity;
	//acceleration += drag;
	vec2 oldP = position;
	vec2 deltaP = ((0.5f*acceleration) * Square(deltaTime)) + (velocity * deltaTime);
	position += deltaP;
}

void GameObject::Update(r32 deltaTime)
{
	isDirty = (rotation != oldRotation);
	RecalculateTransform();
}

void GameObject::RecalculateTransform()
{
	
	if (isDirty)
	{
		oldRotation = rotation;
		forward = Normalize(Vec2(sinf(rotation), cosf(rotation)));
		right = Cross(Vec3(forward, 0), up);
		isDirty = false;
	}
}