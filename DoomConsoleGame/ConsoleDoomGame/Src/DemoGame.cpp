#include "DemoGame.h"
#include "platform.h"
#include "File.h"
#include "Entity.h"
#include "Enemy.h"

std::vector<GameObject*> EntityManager::entities;

enum Tags
{
	Tag_Player,
	Tag_Enemy,
	Tag_Wall,
	Tag_Pillar,
	Tag_PistolAmo,
	Tag_ArmorPickup,
};

u32 AddPlayer(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::EntityCount();
	Player* gameObject = CreateObject(Memory::GetPersistantHandle(), Player);
	gameObject->SetTag(Tag_Player);
	EntityManager::AddEntity(gameObject);
	gameObject->SetPosition(pos);
	gameObject->SetRotation(2.0f);
	gameObject->SetDimensions(Vec2(0.1f, 0.1f));
	gameObject->SetSpeed(500.0f);
	//gameObject->TexIndex = -1;
	//gameObject->isAlive = true;

	return Index;
}

u32 AddEnemy(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::EntityCount();
	Enemy* gameObject = CreateObject(Memory::GetPersistantHandle(), Enemy);
	gameObject->SetTag(Tag_Enemy);
	EntityManager::AddEntity(gameObject);
	gameObject->SetPosition(pos);
	gameObject->SetRotation(2.0f);
	gameObject->SetDimensions(Vec2(0.1f, 0.1f));
	gameObject->SetSpeed(400.0f);
	//gameObject->TexIndex = -1;
	//gameObject->isAlive = true;

	return Index;
}

void AddPillar(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::EntityCount();
	GameObject* gameObject = CreateObject(Memory::GetPersistantHandle(), GameObject);
	gameObject->SetTag(Tag_Pillar);
	EntityManager::AddEntity(gameObject);
	gameObject->SetPosition(pos);
	gameObject->SetDimensions(Vec2(0.25f, 0.25f));
	//gameObject->TexIndex = -1;
}

void AddWall(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::EntityCount();
	GameObject* gameObject = CreateObject(Memory::GetPersistantHandle(), GameObject);
	gameObject->SetTag(Tag_Wall);
	EntityManager::AddEntity(gameObject);
	gameObject->SetPosition(pos);
	gameObject->SetDimensions(Vec2(0.8f, 0.8f));
	//gameObject->TexIndex = -1;
}

void AddPistolAmmo(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::EntityCount();
	GameObject* gameObject = CreateObject(Memory::GetPersistantHandle(), GameObject);
	gameObject->SetTag(Tag_PistolAmo);
	gameObject->SetPosition(pos);
	gameObject->SetDimensions(Vec2(0.5f, 0.5f));
	EntityManager::AddEntity(gameObject);
}

void AddArmorPickup(vec2 pos)
{
	pos += Vec2(0.5f, 0.5f);
	u32 Index = EntityManager::EntityCount();
	GameObject* gameObject = CreateObject(Memory::GetPersistantHandle(), GameObject);
	gameObject->SetTag(Tag_ArmorPickup);
	gameObject->SetPosition(pos);
	gameObject->SetDimensions(Vec2(0.5f, 0.5f));
	EntityManager::AddEntity(gameObject);
}

bool Intersects(vec2 point, vec2 minP, vec2 maxP)
{
	return (point.x > minP.x &&
		point.x < maxP.x &&
		point.y > minP.y &&
		point.y < maxP.y);
}

struct RaycastHitResult
{
	GameObject* entity;
	vec2 point;
	r32 distance;
	u32 entityIndex;
};

bool RayCast(vec2 pos, vec2 dir, RaycastHitResult* HitResult, r32 maxDistance = 16.0f, u32 IgnoreTag = 0)
{
	// Ray cast
	r32 stepSize = 0.1f;
	while (!HitResult->entity && HitResult->distance < maxDistance)
	{
		HitResult->distance += stepSize;

		vec2 testP = { pos.x + dir.x * HitResult->distance, pos.y + dir.y * HitResult->distance };

		for (u32 entityIndex = 0; entityIndex < EntityManager::EntityCount(); ++entityIndex)
		{
			GameObject* testObject = EntityManager::GetEntity(entityIndex);
			if (!testObject->CompareTag(IgnoreTag))
			{
				vec2 minP = testObject->GetPosition() - (testObject->GetDimensions() + Vec2(0.25f, 0.25f));
				vec2 maxP = testObject->GetPosition() + (testObject->GetDimensions() + Vec2(0.25f, 0.25f));

				if(Intersects(testP, minP, maxP))
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

	return false;
}
//
//void MoveEntity(u32 entityIndex, vec2 dir, r32 deltaTime)
//{
//	// Physics Simulation
//	// Collision between the world (WALLS) and the player
//	// Below is collision between pillar obj and the player
//	for (u32 testIndex = 1;
//		testIndex < EntityManager::EntityCount();
//		++testIndex)
//	{
//		if (testIndex != entityIndex)
//		{
//			Entity* testEntity = EntityManager::GetEntity(testIndex);
//			if (testEntity->type != Entity_None)
//			{
//				vec2 minP = testEntity->position - (testEntity->dims + entity->dims);
//				vec2 maxP = testEntity->position + (testEntity->dims + entity->dims);
//
//				if (entity->position.x > minP.x &&
//					entity->position.x < maxP.x &&
//					entity->position.y > minP.y &&
//					entity->position.y < maxP.y)
//				{
//					// Collision
//					entity->position -= deltaP;
//				}
//			}
//		}
//	}
//}
//
//global_variable r32 timer = 0;
//
//void MoveEnemy(u32 entityIndex, u32 targetIndex, Player* playerInfo, r32 deltaTime)
//{
//	// Calculate the right vector based on the Up and the Forward Vector of the player
//	Entity* entity = EntityManager::GetEntity(entityIndex);
//	Entity* target = EntityManager::GetEntity(targetIndex);
//
//	if (target->isAlive)
//	{
//		r32 distance = Length(target->position - entity->position);
//		vec2 direction = {};
//		if (distance < 10.0f && distance >= 2.0f)
//		{
//			if (distance >= 5.0f) // Approach target
//			{
//				entity->forward = Normalize(target->position - entity->position);
//				r32 angle = Dot(entity->forward, player->forward);
//				if (angle <= -0.9f || angle >= 0.0f)
//				{
//					direction.x = 1;
//				}
//				// Move entity
//				MoveEntity(entityIndex, direction, deltaTime);
//			}
//			else if (distance >= 2.0f && distance < 5.0f)
//			{
//				//Attack
//				r32 delay = 0.5f;
//				timer += deltaTime;
//				if (timer >= delay)
//				{
//					if (target->type == EntityType::Entity_Player)
//					{
//						playerInfo->playerDamage(10);
//						target->isAlive = (playerInfo->getHealth() > 0) ? true : false;
//						MoveEntity(targetIndex, Vec2(-1, 0), deltaTime);
//						timer = 0;
//					}
//				}
//			}
//			else
//			{
//				// Backup
//				entity->forward = Normalize(target->position - entity->position);
//				r32 angle = Dot(entity->forward, player->forward);
//				if (angle <= -0.9f || angle >= 0.0f)
//				{
//					direction.x = -1;
//				}
//				// Move entity
//				MoveEntity(entityIndex, direction, deltaTime);
//			}
//		}
//	}
//}

void DemoGame::LoadContent()
{
	// Data from Weapons Class
	weapons[0].setWeaponIndex(WEAPONS::FIST);
	weapons[0].setWeaponName("fist");
	weapons[0].setMaxAmmo(0);
	weapons[0].setAmmo(0);
	
	weapons[1].setWeaponIndex(WEAPONS::PISTOL);
	weapons[1].setWeaponName("pistol");
	weapons[1].setMaxAmmo(50);
	weapons[1].setAmmo(50);

	mapW = 80;
	mapH = 40;

	map =
	{
		L".................#######################.................#######################"
		L".................#................E....#.................#................E....#"
		L".................#..................E..#.................#..................E..#"
		L".................#........P...P........#.................#........P...P........#"
		L".................#...######...######...#.................#...######...######...#"
		L".................#...#....P...P....#...#.................#...#....P...P....#...#"
		L"############.....#...#.............#...#.................#...#.............#...#"
		L"#A........P#######P..############..#...############......#...############..#...#"
		L"#A...................#.............#...#.....AR..P########.....................#"
		L"#R...S...............#.............#...........................................#"
		L"#R........P#######P..#..############.........................#..############...#"
		L"############.....#...#.............#...#.....AR..P########...#.............#...#"
		L".................#...#.............#...############......#...#.............#...#"
		L".................#...############..#...#.................#...############..#...#"
		L".................#...#.............#...#.................#...#.............#...#"
		L".................#...#..P.P........#...#.................#...#..P.P........#...#"
		L".................#...####D##########...#.................#...####D##########...#"
		L".................#......P.P............#.................#......P.P............#"
		L".................#..E..................#.................#..E..................#"
		L".................#....##################.................###..###########..#####"
		L".................#.....................#.................#.....................#"
		L".................#................E....#.................#................E....#"
		L".................#..................E..#.................#..................E..#"
		L".................#........P...P........#.................#........P...P........#"
		L".................#...######...######...#.................#...######...######...#"
		L".................#...#....P...P....#...#.................#...#....P...P....#...#"
		L"############.....#...#.............#...############......#...#.............#...#"
		L"#.........P#######...############..#...#.....AAA.P########...############..#...#"
		L"#.................P..#....................................P....................#"
		L"#....................#.........................................................#"
		L"#.........P#######P..#..############...#.....RRR.P#######.P..#..############...#"
		L"############.....#...#.............#...############......#...#.............#...#"
		L".................#...#.............#...#.................#...#.............#...#"
		L".................#...############..#...#.................#...############..#...#"
		L".................#...#.............#...#.................#...#.............#...#"
		L".................#...#..P.P........#...#.................#...#..P.P........#...#"
		L".................#...####D##########...#.................#...####D##########...#"
		L".................#......P.P............#.................#......P.P............#"
		L".................#..E..................#.................#..E..................#"
		L".................#######################.................#######################"
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

	// Load the ammo pickup
	LoadSprite("data/pickups/ammo_pickup_01.sprt", &ammoPickup);
	LoadSprite("data/pickups/armor_pickup_01.sprt", &armorPickup);

	// Setup player
	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			wchar_t token = map[y * mapW + x];
			if (token == 'S')
			{
				playerIndex = AddPlayer(Vec2(x, y));
				player = (Player*)EntityManager::GetEntity(playerIndex);
				
				player->addWeaponToInventory(&weapons[0]);
				player->addWeaponToInventory(&weapons[1]);
				player->setCurWeapons(WEAPONS::PISTOL);
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
			else if (token == 'A')
			{
				AddPistolAmmo(Vec2(x, y));
			}
			else if (token == 'R')
			{
				AddArmorPickup(Vec2(x, y));
			}
		}
	}

	for (u32 entityIndex = 0;
		entityIndex < EntityManager::EntityCount();
		++entityIndex)
	{
		GameObject* gameObject = EntityManager::GetEntity(entityIndex);
		if (gameObject->CompareTag(Tag_Enemy))
		{
			Enemy* enemy = (Enemy*)gameObject;
			enemy->setTarget(player);
		}
	}

}

void DemoGame::HandleCollision(r32 deltaTime, GameObject* gameObject)
{
	// Physics Simulation
	// Collision between the world (WALLS) and the player
	// Below is collision between pillar obj and the player
	for (u32 testIndex = 0;
		testIndex < EntityManager::EntityCount();
		++testIndex)
	{
		GameObject* testObject = EntityManager::GetEntity(testIndex);
		if (testObject != gameObject)
		{
			vec2 minP = testObject->GetPosition() - (testObject->GetDimensions() + gameObject->GetDimensions());
			vec2 maxP = testObject->GetPosition() + (testObject->GetDimensions() + gameObject->GetDimensions());
			
			if(Intersects(gameObject->GetPosition(), minP, maxP))
			{
				// Collision
				if (!testObject->CompareTag(Tag_PistolAmo) && !testObject->CompareTag(Tag_ArmorPickup))
				{
					gameObject->Move(-gameObject->dir, deltaTime);
					break;
				}
				else 
				{
					if (testObject->CompareTag(Tag_PistolAmo))
					{
						if (gameObject->CompareTag(Tag_Player))
						{
							Player* player = (Player*)gameObject;
							Weapons* weapon = weapons + player->getCurWeapons();
							if (weapon->getAmmo() < weapon->getMaxAmmo())
							{
								weapon->addAmmo(20);
								EntityManager::RemoveEntity(testIndex);
							}
							break;
						}
					}
					if (testObject->CompareTag(Tag_ArmorPickup))
					{
						if (gameObject->CompareTag(Tag_Player))
						{
							Player* player = (Player*)gameObject;
							if (player->getArmor() < 100)
							{
								player->addArmor(20);
								EntityManager::RemoveEntity(testIndex);
							}
							break;
						}
					}
				}
				
			}
		}
	}
}

r32 gameOverTimer = 6.0f;
bool wantsToShoot = false;

bool DemoGame::Update(float deltaTime)
{
	bool Quit = false;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		Quit = true;

	if (player->IsAlive())
	{
		player->dir = Vec2(0, 0); // X: Fwd/Bkwd, Y: StrafeL, StrafeR

		if (Controller)
		{
			// Forward / Backward Movement
			if (Controller->MoveUp.pressed)
			{
				player->dir.x = 1;
			}
			else if (Controller->MoveDown.pressed)
			{
				player->dir.x = -1;
			}

			// Roatting the Angle CCW / CW
			if (Controller->MoveLeft.pressed)
			{
				player->Rotate(-(10 * 0.75f) * deltaTime);
			}
			else if (Controller->MoveRight.pressed)
			{
				player->Rotate((10 * 0.75f) * deltaTime);
			}

			// Straffe Movement
			if (Controller->ActionLeft.pressed)
			{
				player->dir.y = -1;
			}
			else if (Controller->ActionRight.pressed)
			{
				player->dir.y = 1;
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

		player->Update(deltaTime);
		HandleCollision(deltaTime, player);

		// Handle Shooting event
		if (wantsToShoot)
		{
			Weapons* currentWeapon = weapons + player->getCurWeapons();
			if (currentWeapon)
			{
				if (currentWeapon->getAmmo() > 0)
				{
					RaycastHitResult Hit = {};
					if (RayCast(player->GetPosition(), player->GetForward(), &Hit))
					{
						if (Hit.entity)
						{
							Enemy* enemy = (Enemy*)Hit.entity;
							if(enemy)
							{
								if (enemy->IsAlive())
								{
									enemy->takeDamage(20);
								}
								else
								{
									EntityManager::RemoveEntity(Hit.entityIndex);
								}
							}
						}
					}
					currentWeapon->subtractAmmo(1);
				}
			}
			wantsToShoot = false;
		}

		for (u32 entityIndex = 1; entityIndex < EntityManager::EntityCount(); ++entityIndex)
		{
			if (entityIndex != playerIndex)
			{
				GameObject* gameObject = EntityManager::GetEntity(entityIndex);

				if (gameObject->CompareTag(Tag_Enemy))
				{
					// TODO: Update Enemies
					gameObject->Update(deltaTime);
					HandleCollision(deltaTime, gameObject);
				}
			}
		}

		// Clear buffer to certain color
		renderer.ClearBuffer(PIXEL_COLOR_GREY);

		Camera camera = {};
		camera.Position = player->GetPosition();
		camera.rotation = player->GetRotation();
		camera.FOV = PI / 2.0f;
		camera.Depth = 40.0f;

		// Projecting 2D to 3D world
		renderer.ProjectWorld(&camera, map, mapW, mapH, &wall);

		for (u32 entityIndex = 0; entityIndex < EntityManager::EntityCount(); ++entityIndex)
		{
			if (entityIndex != playerIndex)
			{
				GameObject* gameObject = EntityManager::GetEntity(entityIndex);

				switch (gameObject->GetTag())
				{
				case Tag_Pillar:
				{
					renderer.ProjectObject(&camera, gameObject->GetPosition(), &pillar);
				}break;
				case Tag_Enemy:
				{
					Enemy* enemy = (Enemy*)gameObject;
					if(enemy->IsAlive())
						renderer.ProjectObject(&camera, gameObject->GetPosition(), &wall);
				}break;
				case Tag_PistolAmo:
				{
					renderer.ProjectObject(&camera, gameObject->GetPosition(), &ammoPickup);
				}break;
				case Tag_ArmorPickup:
				{
					renderer.ProjectObject(&camera, gameObject->GetPosition(), &armorPickup);
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
		renderer.DrawPixel(player->GetPosition() + screenOnePos, PIXEL_SOLID, PIXEL_COLOR_LIGHT_GREEN);

		// Draw Pillars
		for (u32 entityIndex = 1; entityIndex < EntityManager::EntityCount(); ++entityIndex)
		{
			if (entityIndex != playerIndex)
			{
				GameObject* gameObject = EntityManager::GetEntity(entityIndex);

				switch (gameObject->GetTag())
				{
				case Tag_Pillar:
				{
					renderer.DrawPixel(gameObject->GetPosition() + screenOnePos, PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_RED);
				}break;
				case Tag_Enemy:
				{
					renderer.DrawPixel(gameObject->GetPosition() + screenOnePos, PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_GREEN);
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
				itoa(weapons[player->getCurWeapons()].getAmmo(), ammo, 10);
				ammo[2] = '\0';

				renderer.DrawString(ammo, ArrayCount(ammo), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
				u32 strW = ArrayCount("ammo") * fontDims.x;
				u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
				renderer.DrawString("ammo", ArrayCount("ammo"), font, ArrayCount(font), hudP + text_offset, fontDims);
			}
			else if (i == 1)
			{
				char health[4];
				itoa(player->getHealth(), health, 10);
				health[3] = '\0';

				renderer.DrawString(health, ArrayCount(health), font, ArrayCount(font), hudP + bg_offset + Vec2(10, 0), fontDims);
				u32 strW = ArrayCount("health") * fontDims.x;
				u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
				renderer.DrawString("health", ArrayCount("health"), font, ArrayCount(font), hudP + text_offset, fontDims);
			}
			else if (i == 2)
			{
				renderer.DrawString(weapons[player->getCurWeapons()].getWeaponName(), strlen(weapons[player->getCurWeapons()].getWeaponName()), font, ArrayCount(font), hudP + bg_offset + Vec2(0.5f, 0), fontDims);
				u32 strW = ArrayCount("arms") * fontDims.x;
				u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
				renderer.DrawString("arms", ArrayCount("arms"), font, ArrayCount(font), hudP + text_offset, fontDims);
			}
			else if (i == 4)
			{
				char armor[4];
				itoa(player->getArmor(), armor, 10);
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
	EntityManager::Clear();
}