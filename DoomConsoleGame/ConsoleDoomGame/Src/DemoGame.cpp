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
	player->speed = 500.0f;
	player->TexIndex = -1;
	player->isAlive = true;

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
	enemy->speed = 50.0f;
	enemy->TexIndex = -1;
	enemy->isAlive = true;

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

internal_ bool
TestWall(r32 WallX, r32 RelX, r32 RelY,
	r32 PlayerDeltaX, r32 PlayerDeltaY,
	r32 *tMin, r32 MinY, r32 MaxY)
{
	bool Hit = false;

	float tEpsilon = 0.1f;
	if (PlayerDeltaX != 0)
	{
		// % along Delta vector (Direction)
		float tResult = (WallX - RelX) / PlayerDeltaX;
		// How far we would have gotten total
		float Y = RelY + tResult * PlayerDeltaY; // What Y are we in?

		if (tResult >= 0.0f && *tMin > tResult)
		{
			if (Y >= MinY && Y <= MaxY)
			{
				//Accept new t // Valid Y
				*tMin = Maximum(0, tResult - tEpsilon);
				Hit = true;
			}
		}
	}
	return Hit;
}

void MoveEntity(u32 entityIndex, vec2 dir, r32 deltaTime)
{
	// Calculate the right vector based on the Up and the Forward Vector of the player
	Entity* entity = EntityManager::GetEntity(entityIndex);
	vec3 up = Vec3(0, 0, 1);
	vec3 right = Cross(Vec3(entity->forward, 0), up);
	// Acceleration
	vec2 acceleration = ((entity->forward * dir.x) + (right.xy * dir.y)) * entity->speed;
	//vec2 drag = -0.8f * entity->velocity;
	//acceleration += drag;
	vec2 oldP = entity->position;
	vec2 deltaP = ((0.5f*acceleration) * Square(deltaTime)) + (entity->velocity * deltaTime);
	entity->position += deltaP;

	// Physics Simulation
	// Collision between the world (WALLS) and the player
	// Below is collision between pillar obj and the player
	for (u32 testIndex = 1;
		testIndex < EntityManager::EntityCount();
		++testIndex)
	{
		if (testIndex != entityIndex)
		{
			Entity* testEntity = EntityManager::GetEntity(testIndex);

			vec2 minP = testEntity->position - (testEntity->dims + entity->dims);
			vec2 maxP = testEntity->position + (testEntity->dims + entity->dims);

			if (entity->position.x > minP.x &&
				entity->position.x < maxP.x &&
				entity->position.y > minP.y &&
				entity->position.y < maxP.y)
			{
				// Collision
				entity->position -= deltaP;
			}
		}
	}
}

global_variable r32 timer = 0;

void MoveEnemy(u32 entityIndex, u32 targetIndex, Player* playerInfo, r32 deltaTime)
{
	// Calculate the right vector based on the Up and the Forward Vector of the player
	Entity* entity = EntityManager::GetEntity(entityIndex);
	Entity* target = EntityManager::GetEntity(targetIndex);

	if (target->isAlive)
	{
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
				r32 delay = 1.0f;
				timer += deltaTime;
				if (timer >= delay)
				{
					if (target->type == EntityType::Entity_Player)
					{
						playerInfo->playerDamage(10);
						target->isAlive = (playerInfo->getHealth() > 0) ? true : false;
						MoveEntity(targetIndex, Vec2(-1, 0), deltaTime);
						timer = 0;
					}
				}
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
}

void DemoGame::LoadContent()
{
	// Data from Player Class
	playerInfo.setMaxHealth();
	playerInfo.setMaxArmor();
	playerInfo.setHealth(100);
	playerInfo.setArmor(50);
	
	// Data from Weapons Class
	weaponInfo.setWeaponIndex(WEAPONS::PISTOL);
	weaponInfo.setWeaponName("pistol");
	weaponInfo.setMaxAmmo(50);
	weaponInfo.setAmmo(50);

	playerInfo.addWeaponToInventory(weaponInfo);
	playerInfo.setCurWeapons(WEAPONS::PISTOL);

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

r32 gameOverTimer = 6.0f;
bool wantsToShoot = false;

bool DemoGame::Update(float deltaTime)
{
	bool Quit = false;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		Quit = true;

	vec2 direction = {}; // X: Fwd/Bkwd, Y: StrafeL, StrafeR

	if (player->isAlive)
	{
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
				player->rotation -= (10 * 0.75f) * deltaTime;
			}
			else if (Controller->MoveRight.pressed)
			{
				player->rotation += (10 * 0.75f) * deltaTime;
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

			// Handle shooting
			if (!wantsToShoot)
			{
				if (Controller->RightTrigger.pressed)
				{
					wantsToShoot = true;
				}
			}
		}

		MoveEntity(playerIndex, direction, deltaTime);

		if (wantsToShoot)
		{
			// Ray cast
			Entity* HitEntity = 0;
			vec2 HitPoint = {};
			u32 Index = 0;
			r32 stepSize = 0.1f;
			r32 distance = 0;

			vec2 RayDirection = player->forward;

			while(!HitEntity && distance < 16)
			{
				distance += stepSize;

				vec2 testP = {player->position.x + RayDirection.x * distance, player->position.y + RayDirection.y * distance };
				
				for (u32 entityIndex = 1; entityIndex < EntityManager::EntityCount(); ++entityIndex)
				{
					if (entityIndex != playerIndex)
					{
						Entity* testEntity = EntityManager::GetEntity(entityIndex);
						vec2 minP = testEntity->position - (testEntity->dims);
						vec2 maxP = testEntity->position + (testEntity->dims);

						if (testP.x > minP.x &&
							testP.x < maxP.x &&
							testP.y > minP.y &&
							testP.y < maxP.y)
						{
							// Collision
							HitEntity = testEntity;
							HitPoint = testP;
							Index = entityIndex;
							break;
						}
					}
				}

				if (HitEntity)
					break;
			}

			if (HitEntity)
			{
				if (HitEntity->isAlive)
				{
					HitEntity->isAlive = false;
					EntityManager::RemoveEntity(Index);
				}
			}

			weaponInfo.subtractAmmo(1);
			wantsToShoot = false;
		}

		for (u32 entityIndex = 1; entityIndex < EntityManager::EntityCount(); ++entityIndex)
		{
			if (entityIndex != playerIndex)
			{
				Entity* entity = EntityManager::GetEntity(entityIndex);

				if (entity->isAlive)
				{
					switch (entity->type)
					{
					case Entity_Enemy:
					{
						MoveEnemy(entityIndex, playerIndex, &playerInfo, deltaTime);
					}break;
					default: {}
					}
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
				char ammo[3];
				itoa(playerInfo.getCurWeapons().getAmmo(), ammo, 10);
				ammo[2] = '\0';

				renderer.DrawString(ammo, ArrayCount(ammo), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
				u32 strW = ArrayCount("ammo") * fontDims.x;
				u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
				renderer.DrawString("ammo", ArrayCount("ammo"), font, ArrayCount(font), hudP + text_offset, fontDims);
			}
			else if (i == 1)
			{
				char health[4];
				itoa(playerInfo.getHealth(), health, 10);
				health[3] = '\0';

				renderer.DrawString(health, ArrayCount(health), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
				u32 strW = ArrayCount("health") * fontDims.x;
				u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
				renderer.DrawString("health", ArrayCount("health"), font, ArrayCount(font), hudP + text_offset, fontDims);
			}
			else if (i == 2)
			{
				renderer.DrawString(playerInfo.getCurWeapons().getWeaponName(), strlen(playerInfo.getCurWeapons().getWeaponName()), font, ArrayCount(font), hudP + bg_offset + Vec2(0.5f, 0), fontDims);
				u32 strW = ArrayCount("arms") * fontDims.x;
				u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
				renderer.DrawString("arms", ArrayCount("arms"), font, ArrayCount(font), hudP + text_offset, fontDims);
			}
			else if (i == 4)
			{
				char armor[4];
				itoa(playerInfo.getArmor(), armor, 10);
				armor[3] = '\0';

				renderer.DrawString(armor, ArrayCount(armor), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
				u32 strW = ArrayCount("armor") * fontDims.x;
				u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
				renderer.DrawString("armor", ArrayCount("armor"), font, ArrayCount(font), hudP + text_offset, fontDims);
			}
		}
	}
	else
	{
		renderer.ClearBuffer(PIXEL_COLOR_DARK_RED);
		vec2 dims = {20, 20};
		vec2 pos = (Vec2(renderer.GetRenderBuffers()->Width, renderer.GetRenderBuffers()->Height) * 0.5f) - (Vec2(dims.x*ArrayCount("game over"), dims.y) * 0.5f);
		renderer.DrawString("game over", ArrayCount("game over"), font, ArrayCount(font), pos, dims);
		
		char timerBuff[2] = {};
		itoa(gameOverTimer, timerBuff, 10);
		renderer.DrawString(timerBuff, ArrayCount(timerBuff), font, ArrayCount(font), pos + Vec2(8*ArrayCount("game over"), dims.y), dims);

		gameOverTimer -= deltaTime;
		if (gameOverTimer <= 0.0f)
		{
			Quit = true;
		}
	}
	// Present buffers to the screen
	renderer.PresentBuffer();

	return Quit;
}

void DemoGame::UnloadContent()
{

}