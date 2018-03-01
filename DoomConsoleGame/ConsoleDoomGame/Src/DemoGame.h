/*******************************************************
FileName: DemoGame.h
StartDate: 01/20/2018
ModDate: 02/27/2018
CreatorName: George Wulfers
ContributorsName: George Wulfers
===================================================
All of the code for the game will be held here within this class.
*******************************************************/
#ifndef DEMO_GAME_H
#define DEMO_GAME_H

#include "BaseGame.h"
#include "Sprite.h"
#include "vec2.h"

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

	vec2 playerP;
	float playerAngle;
	float FOV;
	float depth;

	vec2 pillarP;
	Sprite pillar;
	Sprite wall;
	Sprite hudBG;

	vec2 hudP;
	vec2 hudDims;

};

#endif
