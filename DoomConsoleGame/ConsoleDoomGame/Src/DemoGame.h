#ifndef DEMO_GAME_H
#define DEMO_GAME_H

#include "BaseGame.h"

class DemoGame : public BaseGame
{
public:
	DemoGame() {}
	virtual ~DemoGame() {}

	virtual void LoadContent() override;
	virtual bool Update(float deltaTime) override;
	virtual void UnloadContent() override;

protected:
	int mapW;
	int mapH;
	wchar_t *map;

	float playerX;
	float playerY;
	float playerAngle;
	float FOV;
	float depth;

	float SpriteX;
	float SpriteY;
	int SpriteW;
	int SpriteH;
	wchar_t *Sprite;
};

#endif
