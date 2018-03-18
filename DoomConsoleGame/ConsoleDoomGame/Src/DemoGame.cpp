#include "DemoGame.h"
#include "platform.h"
#include "File.h"
#include "Entity.h"

u32 EntityManager::entityCount = 0;
Entity EntityManager::entities[4096] = {};

Entity* player = 0;
u32 playerIndex = 0;

u32 AddPlayer(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::AddEntity(Entity_Player);
	Entity* player = EntityManager::GetEntity(Index);
	player->position = pos;
	player->rotation = 2.0f;
	player->dims = Vec2(0.1f, 0.1f);
	player->speed = 10.0f;
	player->TexIndex = -1;

	return Index;
}

u32 AddEnemy(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::AddEntity(Entity_Enemy);
	Entity* enemy = EntityManager::GetEntity(Index);
	enemy->position = pos;
	enemy->rotation = 2.0f;
	enemy->dims = Vec2(0.1f, 0.1f);
	enemy->speed = 5.0f;
	enemy->TexIndex = -1;

	return Index;
}

void AddPillar(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::AddEntity(Entity_Pillar);
	Entity* entity = EntityManager::GetEntity(Index);
	entity->position = pos;
	entity->dims = Vec2(0.25f, 0.25f);
	entity->TexIndex = -1;
}

void AddWall(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::AddEntity(Entity_Wall);
	Entity* entity = EntityManager::GetEntity(Index);
	entity->position = pos;
	entity->dims = Vec2(0.8f, 0.8f);
	entity->TexIndex = -1;
}

void MoveEntity(u32 entityIndex, vec2 dir, r32 deltaTime)
{
	// Calculate the right vector based on the Up and the Forward Vector of the player
	Entity* entity = EntityManager::GetEntity(entityIndex);
	vec3 up = Vec3(0, 0, 1);
	vec3 right = Cross(Vec3(entity->forward, 0), up);
	entity->position += ((entity->forward * dir.x) + (right.xy * dir.y)) * entity->speed * deltaTime;

	// Physics Simulation
	// Collision between the world (WALLS) and the player
	// Below is collision between pillar obj and the player
	for (u32 testIndex = 1; testIndex < EntityManager::EntityCount(); ++testIndex)
	{
		if (testIndex != entityIndex)
		{
			Entity* testEntity = EntityManager::GetEntity(testIndex);

			vec2 minP = testEntity->position - (testEntity->dims + entity->dims);
			vec2 maxP = testEntity->position + (testEntity->dims + entity->dims);

			if (entity->position.x > minP.x &&
				entity->position.x < maxP.x &&
				entity->position.y > minP.y &&
				entity->position.y < maxP.y /*||
											map[(int)player->position.y * mapW + (int)player->position.x] == L'#'*/)
			{
				// Collision
				entity->position -= ((entity->forward * dir.x) + (right.xy * dir.y)) * entity->speed * deltaTime;
			}
		}
	}
}

void MoveEnemy(u32 entityIndex, u32 targetIndex, r32 deltaTime)
{
	// Calculate the right vector based on the Up and the Forward Vector of the player
	Entity* entity = EntityManager::GetEntity(entityIndex);
	Entity* target = EntityManager::GetEntity(targetIndex);

	r32 distance = Length(target->position - entity->position);
	vec2 direction = {};
	if (distance < 10.0f && distance >= 2.0f)
	{
		if (distance >= 5.0f) // Approach target
		{
			entity->forward = Normalize(target->position - entity->position);
			r32 angle = Dot(entity->forward, player->forward);
			if (angle <= -0.9f || angle >= 0.0f)
			{
				direction.x = 1;
			}
			// Move entity
			MoveEntity(entityIndex, direction, deltaTime);
		}
		else if (distance >= 2.0f && distance < 5.0f)
		{
			//Attack

		}
		else
		{
			// Backup
			entity->forward = Normalize(target->position - entity->position);
			r32 angle = Dot(entity->forward, player->forward);
			if (angle <= -0.9f || angle >= 0.0f)
			{
				direction.x = -1;
			}
			// Move entity
			MoveEntity(entityIndex, direction, deltaTime);
		}
	}
}

void DemoGame::LoadContent()
{
	mapW = 40;
	mapH = 20;

	map =
	{
		L".................#######################"
		L".................#................E....#"
		L".................#..................E..#"
		L".................#........P...P........#"
		L".................#...######...######...#"
		L".................#...#....P...P....#...#"
		L".................#...#.............#...#"
		L"############.....#...############..#...#"
		L"#.........P#######P..#.............#...#"
		L"#....S...............#.............#...#"
		L"#.........P#######P..#..############...#"
		L"############.....#...#.............#...#"
		L".................#...#.............#...#"
		L".................#...############..#...#"
		L".................#...#.............#...#"
		L".................#...#..P.P........#...#"
		L".................#...####D##########...#"
		L".................#......P.P............#"
		L".................#..E..................#"
		L".................#######################"
	};

	// Load Fonts
	char c = 'a';
	for (int i = 0; i < ArrayCount(font); i++)
	{
		char fid[2] = { c, 0 };
		char buffer[256] = {};
		strcat(buffer, "data/fonts/font_");
		strcat(buffer, fid);
		strcat(buffer, ".sprt");

		LoadSprite(buffer, font + i);
		if (c == 'z')
		{
			c = '0';
			continue;
		}
		c++;
	}
	fontDims = Vec2(8, 8);

	// Load the pillar texture
	LoadSprite("data/pillar_01.sprt", &pillar);
	
	// Load the wall texture
	LoadSprite("data/wall_01.sprt", &wall);

	// Load the hud background texture
	LoadSprite("data/hud_bg.sprt", &hudBG);
	i32 w = RoundReal32ToInt32(renderer.GetRenderBuffers()->Width / 7.0f);
	hudDims = Vec2((r32)w, 20);
	hudP = Vec2(0, (r32)renderer.GetRenderBuffers()->Height - hudDims.y);

	EntityManager::AddEntity(Entity_None);	// Null Entity

	// Setup player
	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			wchar_t token = map[y * mapW + x];
			if (token == 'S')
			{
				playerIndex = AddPlayer(Vec2(x, y));
				player = EntityManager::GetEntity(playerIndex);
			}
			else if (token == 'P')
			{
				AddPillar(Vec2(x, y));
			}
			else if (token == '#')
			{
				AddWall(Vec2(x, y));
			}
			else if (token == 'E')
			{
				AddEnemy(Vec2(x, y));
			}
		}
	}

	renderer.BuildWorldHash();
}

bool DemoGame::Update(float deltaTime)
{
	bool Quit = false;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		Quit = true;

	vec2 direction = {}; // X: Fwd/Bkwd, Y: StrafeL, StrafeR

	if (Controller)
	{
		// Forward / Backward Movement
		if (Controller->MoveUp.pressed)
		{
			direction.x = 1;
		}
		else if (Controller->MoveDown.pressed)
		{
			direction.x = -1;
		}

		// Roatting the Angle CCW / CW
		if (Controller->MoveLeft.pressed)
		{
			player->rotation -= (player->speed * 0.75f) * deltaTime;
		}
		else if (Controller->MoveRight.pressed)
		{
			player->rotation += (player->speed * 0.75f) * deltaTime;
		}

		player->forward = Normalize(Vec2(sinf(player->rotation), cosf(player->rotation)));

		// Straffe Movement
		if (Controller->ActionLeft.pressed)
		{
			direction.y = -1;
		}
		else if (Controller->ActionRight.pressed)
		{
			direction.y = 1;
		}
	}

	MoveEntity(playerIndex, direction, deltaTime);

	for (u32 entityIndex = 1; entityIndex < EntityManager::EntityCount(); ++entityIndex)
	{
		if (entityIndex != playerIndex)
		{
			Entity* entity = EntityManager::GetEntity(entityIndex);

			switch (entity->type)
			{
			case Entity_Enemy:
			{
				MoveEnemy(entityIndex, playerIndex, deltaTime);
			}break;
			default: {}
			}
		}
	}

	// Clear buffer to certain color
	renderer.ClearBuffer(PIXEL_COLOR_GREY);

	Camera camera = {};
	camera.Position = player->position;
	camera.rotation = player->rotation;
	camera.FOV = PI / 2.0f;
	camera.Depth = 40.0f;
	
	// Projecting 2D to 3D world
	renderer.ProjectWorld(&camera, map, mapW, mapH, &wall);

	for (u32 entityIndex = 1; entityIndex < EntityManager::EntityCount(); ++entityIndex)
	{
		if (entityIndex != playerIndex)
		{
			Entity* entity = EntityManager::GetEntity(entityIndex);

			switch (entity->type)
			{
			case Entity_Pillar:
			{
				renderer.ProjectObject(&camera, entity->position, &pillar);
			}break;
			case Entity_Enemy:
			{
				renderer.ProjectObject(&camera, entity->position, &wall);
			}break;
			default: {}
			}
		}
	}

	// HUD Routines Below
	// 2D Map
	vec2 screenOnePos = Vec2(10, 10);
	
	// Draw Map
	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			vec2 p = Vec2(x, y);
			if (map[y * mapW + x] == '#')
			{
				renderer.DrawPixel(screenOnePos + p, PIXEL_SOLID, PIXEL_COLOR_DARK_RED); // Walls
			}
			else
			{
				renderer.DrawPixel(screenOnePos + p, PIXEL_SOLID, PIXEL_COLOR_GREY); // Floor
			}
		}
	}

	// Drawing View Cone
	for (u32 x = 0; x < mapW; ++x)
	{
		r32 ViewAngle = (camera.rotation - camera.FOV / 2) + ((r32)x / mapW) * camera.FOV;
		vec2 eye = { sinf(ViewAngle), cosf(ViewAngle) };
		for (u32 y = 0; y < mapH; ++y)
		{
			renderer.DrawPixel(camera.Position + screenOnePos + eye * y, PIXEL_SOLID, PIXEL_COLOR_LIGHT_BLUE);
		}
	}

	// Draw player
	renderer.DrawPixel(player->position + screenOnePos, PIXEL_SOLID, PIXEL_COLOR_LIGHT_GREEN);

	// Draw Pillars
	for (u32 entityIndex = 1; entityIndex < EntityManager::EntityCount(); ++entityIndex)
	{
		if (entityIndex != playerIndex)
		{
			Entity* entity = EntityManager::GetEntity(entityIndex);

			switch (entity->type)
			{
			case Entity_Pillar:
			{
				renderer.DrawPixel(entity->position + screenOnePos, PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_RED);
			}break;
			case Entity_Enemy:
			{
				renderer.DrawPixel(entity->position + screenOnePos, PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_GREEN);
			}break;
			default: {}
			}
		}
	}

	// Drawing UI The dumb way
	for (u32 i = 0; i < 7; i++)
	{
		vec2 bg_offset = Vec2(i * (hudDims.x), 0);
		vec2 text_offset = Vec2(i * (hudDims.x), 10);
		renderer.DrawUI(hudP + bg_offset, hudDims, &hudBG);
		if (i == 0)
		{
			renderer.DrawString("0", ArrayCount("0"), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
			u32 strW = ArrayCount("ammo") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			renderer.DrawString("ammo", ArrayCount("ammo"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 1)
		{
			renderer.DrawString("100", ArrayCount("100"), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
			u32 strW = ArrayCount("health") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			renderer.DrawString("health", ArrayCount("health"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 2)
		{
			u32 strW = ArrayCount("arms") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			renderer.DrawString("arms", ArrayCount("arms"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 4)
		{
			renderer.DrawString("100", ArrayCount("100"), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
			u32 strW = ArrayCount("armor") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			renderer.DrawString("armor", ArrayCount("armor"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
	}

	// Present buffers to the screen
	renderer.PresentBuffer();

	return Quit;
}

void DemoGame::UnloadContent()
{

}