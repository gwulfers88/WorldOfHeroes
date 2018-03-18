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
#ifndef DEMO_GAME_H
#define DEMO_GAME_H

#include "BaseGame.h"
#include "Sprite.h"
#include "vec3.h"
#include "Player.h"

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

	Sprite pillar;
	Sprite wall;
	Sprite hudBG;

	vec2 hudP;
	vec2 hudDims;

	Player playerInfo;
	Weapons weaponInfo;

	Sprite font[36];
	vec2 fontDims;
};

#endif
