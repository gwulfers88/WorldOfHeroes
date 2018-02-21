#ifndef DEMO_GAME_H
#define DEMO_GAME_H

#include "BaseGame.h"
#include "Sprite.h"

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

	float pillarX;
	float pillarY;

	Sprite pillar;
};

#endif
