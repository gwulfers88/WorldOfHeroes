#include "DemoGame.h"
#include "platform.h"
#include "File.h"

struct Entity
{
	vec2 p;
	vec2 dims;
	vec2 forward;
	float rotation;
};

Entity player = {};

// This function prints strings using our font.
void printString(ConsoleRenderer renderer, char* text, u32 textCount, Sprite* font, u32 fontCount, vec2 pos, vec2 dims)
{
	u32 row = 0;
	for (int i = 0; i < textCount; i++)
	{
		char c = text[i];
		int index = -1;

		if (c >= 'a' && c <= 'z')
		{
			index = c - 'a';
		}
		else if (c >= '0' && c <= '9')
		{
			index = c - '0';
			index += 26;
		}

		if (index < fontCount)
		{
			u32 x = (i * dims.x);
			u32 y = (row * dims.y);
			renderer.DrawUI(pos + Vec2(x, y), dims, font + index);
		}
	}
}

void DemoGame::LoadContent()
{
#if 0
	playerP = Vec2(1, 1);
	playerAngle = 0;
#else
	player.rotation = 0;
#endif
	FOV = PI / 4.0f;
	depth = 40.0f;

	mapW = 40;
	mapH = 20;

	map =
	{
		L"########################################"
		L"#..........D.....D.....................#"
		L"#..........#######...#######D#######...#"
		L"#..........#.....#...#.............#...#"
		L"#......S...#.....#...######...######...#"
		L"############.....#...#.............#...#"
		L".................#...#.............#...#"
		L".................#...############..#...#"
		L".................#...#.............#...#"
		L".................D...#.............#...#"
		L".................D...#..############...#"
		L".................#...#.............#...#"
		L".................#...#.............#...#"
		L".................#...############..#...#"
		L".................#...#.............#...#"
		L".................#...#.............#...#"
		L".................#...####D##########...#"
		L".................#.....................#"
		L".................#.....................#"
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
	pillarP = Vec2(27.5f, 5.5f);
	
	// Load the wall texture
	LoadSprite("data/wall_01.sprt", &wall);

	// Load the hud background texture
	LoadSprite("data/hud_bg.sprt", &hudBG);
	i32 w = RoundReal32ToInt32(renderer.GetRenderBuffers()->Width / 7.0f);
	hudDims = Vec2((r32)w, 20);
	hudP = Vec2(0, (r32)renderer.GetRenderBuffers()->Height - hudDims.y);

	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			if (map[y * mapW + x] == 'S')
			{
#if 0
				playerP = Vec2(x, y);
#else
				player.p = Vec2((r32)x, (r32)y);
				player.dims = Vec2(0.5f, 0.5f);
#endif
			}
		}
	}
}

bool DemoGame::Update(float deltaTime)
{
	bool Quit = false;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		Quit = true;

	float playerSpeed = 10.0f;

	if (Controller)
	{
		// Forward / Backward Movement
		if (Controller->MoveUp.pressed)
		{
#if 0
			playerP.x += sinf(playerAngle) * playerSpeed * deltaTime;
			playerP.y += cosf(playerAngle) * playerSpeed * deltaTime;
			if (map[(int)playerP.y * mapW + (int)playerP.x] == L'#')
			{
				playerP.x -= sinf(playerAngle) * playerSpeed * deltaTime;
				playerP.y -= cosf(playerAngle) * playerSpeed * deltaTime;
			}
#else
			player.p += player.forward * playerSpeed * deltaTime;
			if (map[(int)player.p.y * mapW + (int)player.p.x] == L'#')
			{
				player.p -= player.forward * playerSpeed * deltaTime;
			}
#endif
		}
		else if (Controller->MoveDown.pressed)
		{
#if 0 
			playerP.x -= sinf(playerAngle) * playerSpeed * deltaTime;
			playerP.y -= cosf(playerAngle) * playerSpeed * deltaTime;
			if (map[(int)playerP.y * mapW + (int)playerP.x] == L'#')
			{
				playerP.x += sinf(playerAngle) * playerSpeed * deltaTime;
				playerP.y += cosf(playerAngle) * playerSpeed * deltaTime;
			}
#else
			player.p -= player.forward * playerSpeed * deltaTime;
			if (map[(int)player.p.y * mapW + (int)player.p.x] == L'#')
			{
				player.p += player.forward * playerSpeed * deltaTime;
			}
#endif
		}

		// Roatting the Angle CCW / CW
		if (Controller->MoveLeft.pressed)
		{
#if 0
			playerAngle -= (playerSpeed * 0.75f) * deltaTime;
#else
			player.rotation -= (playerSpeed * 0.75f) * deltaTime;
#endif
		}
		else if (Controller->MoveRight.pressed)
		{
#if 0
			playerAngle += (playerSpeed * 0.75f) * deltaTime;
#else
			player.rotation += (playerSpeed * 0.75f) * deltaTime;
#endif
		}

		player.forward = Normalize(Vec2(sinf(player.rotation), cosf(player.rotation)));

		vec3 up = Vec3(0, 0, 1);
		vec3 right = Cross(Vec3(player.forward, 0), up);

		// Straffe Movement
		if (Controller->ActionLeft.pressed)
		{
#if 0
			playerP.x -= cosf(playerAngle) * playerSpeed * deltaTime;
			playerP.y += sinf(playerAngle) * playerSpeed * deltaTime;
			if (map[(int)playerP.y * mapW + (int)playerP.x] == L'#')
			{
				playerP.x += cosf(playerAngle) * playerSpeed * deltaTime;
				playerP.y -= sinf(playerAngle) * playerSpeed * deltaTime;
			}
#else
			player.p += -right.xy * playerSpeed * deltaTime;
			if (map[(int)player.p.y * mapW + (int)player.p.x] == L'#')
			{
				player.p += right.xy * playerSpeed * deltaTime;
			}
#endif
		}
		else if (Controller->ActionRight.pressed)
		{
#if 0
			playerP.x += cosf(playerAngle) * playerSpeed * deltaTime;
			playerP.y -= sinf(playerAngle) * playerSpeed * deltaTime;
			if (map[(int)playerP.y * mapW + (int)playerP.x] == L'#')
			{
				playerP.x -= cosf(playerAngle) * playerSpeed * deltaTime;
				playerP.y += sinf(playerAngle) * playerSpeed * deltaTime;
			}
#else
			player.p += right.xy * playerSpeed * deltaTime;
			if (map[(int)player.p.y * mapW + (int)player.p.x] == L'#')
			{
				player.p += -right.xy * playerSpeed * deltaTime;
			}
#endif
		}

	}

	vec2 pillarDims = Vec2(0.25f, 0.25f) + player.dims;
	vec2 minPillarP = pillarP - pillarDims;
	vec2 maxPillarP = pillarP + pillarDims;

	if (player.p.x > minPillarP.x &&
		player.p.x < maxPillarP.x &&
		player.p.y > minPillarP.y &&
		player.p.y < maxPillarP.y)
	{
		// Collision
		player.p -= player.forward * playerSpeed * deltaTime;
	}

	// Clear buffer to certain color
	renderer.ClearBuffer(PIXEL_COLOR_GREY);

	// Ray Casting Routine
	int screenWidth = renderer.GetRenderBuffers()->Width;
	int screenHeight = renderer.GetRenderBuffers()->Height;

	for (int x = 0; x < renderer.GetRenderBuffers()->Width; ++x)
	{
		// Find the ray angle based on the players Rotattion angle
		//float RayAngle = (playerAngle - FOV / 2) + ((float)x / screenWidth) * FOV;
		float RayAngle = (player.rotation - FOV / 2) + ((float)x / screenWidth) * FOV;

		float StepSize = 0.01f;			// This is how far we will advanced the ray per iteration
		float DistanceToWall = 0.0f;	// 

		bool HitWall = false;
		bool HitDoor = false;

		vec2 eye = {};
		eye.x = sinf(RayAngle);
		eye.y = cosf(RayAngle);

		r32 sampleX = 0;

		short Color = PIXEL_COLOR_WHITE;

		while (!HitWall && !HitDoor && DistanceToWall < depth)
		{
			DistanceToWall += StepSize;

			i32 TestX = (player.p.x + eye.x * DistanceToWall);
			i32 TestY = (player.p.y + eye.y * DistanceToWall);

			vec2 deltaP = player.p + eye * DistanceToWall;

			// Our ray has gone Out of bounds
			if (TestX < 0 || TestX >= mapW || TestY < 0 || TestY >= mapH)
			{
				HitWall = true;
				DistanceToWall = depth;
			}
			else
			{
				// Ray is still in bounds to test cell for walls
				if (map[(i32)TestY * mapW + (i32)TestX] == L'#')
				{
					// Ray has hit a wall
					HitWall = true;

					// Calculate the Texture coordinates here.
					Color = PIXEL_COLOR_DARK_CYAN;

					vec2 wallMidP = Vec2((r32)TestX + 0.5f, (r32)TestY + 0.5f);
					vec2 testP = player.p + eye * DistanceToWall;
					float testAngle = atan2f(testP.y - wallMidP.y, testP.x - wallMidP.x);

					if (testAngle >= -PI * 0.25f && testAngle < PI * 0.25f)
						sampleX = testP.y - TestY;
					if (testAngle >= PI * 0.25f && testAngle < PI * 0.75f)
						sampleX = testP.x - TestX;
					if (testAngle < -PI * 0.25f && testAngle >= -PI * 0.75f)
						sampleX = testP.x - TestX;
					if (testAngle >= PI * 0.75f || testAngle < -PI * 0.75f)
						sampleX = testP.y - TestY;
				}
				// Ray is still in bounds to test cell for Doors
				else if (map[(i32)TestY * mapW + (i32)TestX] == L'D')
				{
					vec2 doorMidP = Vec2((r32)TestX + 0.5f, (r32)TestY + 0.5f);
					
					if (deltaP.x > doorMidP.x - 0.05f && deltaP.x < doorMidP.x + 0.05f)
					{
						// Calculate the Texture coordinates here.
						Color = PIXEL_COLOR_DARK_MAGENTA;
						
						vec2 testP = player.p + eye * DistanceToWall;
						float testAngle = atan2f(testP.y - doorMidP.y, testP.x - doorMidP.x);

						if (testAngle >= -PI * 0.25f && testAngle < PI * 0.25f)
							sampleX = testP.y - TestY;
						if (testAngle >= PI * 0.25f && testAngle < PI * 0.75f)
							sampleX = testP.x - TestX;
						if (testAngle < -PI * 0.25f && testAngle >= -PI * 0.75f)
							sampleX = testP.x - TestX;
						if (testAngle >= PI * 0.75f || testAngle < -PI * 0.75f)
							sampleX = testP.y - TestY;

						// Ray has hit a wall
						HitDoor = true;
					}
				}
			}
		}

		// Calculate the distance to the ceiling
		int DistanceToCeiling = (((float)screenHeight * 0.5f) - ((float)screenHeight / DistanceToWall));
		int DistanceToFloor = (screenHeight - DistanceToCeiling);
		
		renderer.GetRenderBuffers()->DepthBuffer[x] = DistanceToWall;

		// Shade walls based on distance 
		wchar_t shade = PIXEL_SOLID;
		
		if (DistanceToWall <= depth / 16)
		{
			shade = PIXEL_SOLID;
		}
		else if (DistanceToWall < depth / 8)
		{
			shade = PIXEL_SEMI_DARK;
		}
		else if (DistanceToWall < depth / 4)
		{
			shade = PIXEL_MEDIUM_DARK;
		}
		else if (DistanceToWall < depth / 2)
		{
			shade = PIXEL_DARK;
		}
		else
			shade = ' ';

		for (int y = 0; y < screenHeight; ++y)
		{
			if (y <= DistanceToCeiling)
			{
				renderer.DrawPixel({(r32)x, (r32)y}, PIXEL_SOLID, PIXEL_COLOR_BLACK);
			}
			else if (y > DistanceToCeiling && y <= DistanceToFloor)	// Wall
			{
				if (HitWall)
				{
					r32 sampleY = ((r32)y - DistanceToCeiling) / (DistanceToFloor - DistanceToCeiling);
					Color = SampleSprite(Vec2(sampleX, sampleY), &wall).Color;
					renderer.DrawPixel({ (r32)x, (r32)y }, shade, Color);
				}
				if (HitDoor)
				{
					renderer.DrawPixel({ (r32)x, (r32)y }, shade, Color);
				}
			}
			else
			{
				// Shade floor based on distance
				float b = 1.0f - (((float)y - screenHeight * 0.5f) / ((float)screenHeight * 0.5f));
				if (b < 0.25)
				{
					shade = PIXEL_SOLID;
					Color = PIXEL_COLOR_DARK_GREEN;
				}
				else if (b < 0.5)
				{
					shade = PIXEL_SEMI_DARK;
					Color = PIXEL_COLOR_DARK_GREEN;
				}
				else if (b < 0.75)
				{
					shade = PIXEL_MEDIUM_DARK;
					Color = PIXEL_COLOR_DARK_GREEN;
				}
				else if (b < 0.9)
				{
					shade = PIXEL_DARK;
					Color = PIXEL_COLOR_DARK_GREEN;
				}
				renderer.DrawPixel({(r32)x, (r32)y}, shade, Color);
			}
		}
	}

	// drawing Objects
	renderer.ProjectObject(player.p, player.rotation, FOV, depth, pillarP, &pillar);
	renderer.ProjectObject(player.p, player.rotation, FOV, depth, pillarP + Vec2(1, 0), &pillar);
	renderer.ProjectObject(player.p, player.rotation, FOV, depth, pillarP + Vec2(0, 1), &pillar);
	renderer.ProjectObject(player.p, player.rotation, FOV, depth, pillarP + Vec2(1, 1), &pillar);

	/// HUD Routines Below
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
				renderer.DrawPixel(screenOnePos + p, PIXEL_SOLID, PIXEL_COLOR_GREY); // Floor
		}
	}

	// This will draw the players field of view on the 2D map.
	for (int x = 0; x < mapW; ++x)
	{
		float ViewAngle = (player.rotation - FOV / 2) + ((float)x / mapW) * FOV;
		vec2 eye = {};
		eye.x = sinf(ViewAngle);
		eye.y = cosf(ViewAngle);

		for (int y = 0; y < mapH; ++y)
		{
			renderer.DrawPixel(player.p + screenOnePos + eye * y, PIXEL_SOLID, PIXEL_COLOR_LIGHT_BLUE);
		}
	}

	// Draw player
	renderer.DrawPixel(player.p + screenOnePos, PIXEL_SOLID, PIXEL_COLOR_LIGHT_GREEN);

	// Draw Pillar
	renderer.DrawPixel(pillarP + screenOnePos, PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_RED);

	// Drawing UI The dumb way
	for (int i = 0; i < 7; i++)
	{
		vec2 bg_offset = Vec2(i * (hudDims.x), 0);
		vec2 text_offset = Vec2(i * (hudDims.x), 10);
		renderer.DrawUI(hudP + bg_offset, hudDims, &hudBG);
		if (i == 0)
		{
			printString(renderer, "0", ArrayCount("0"), font, ArrayCount(font), hudP + bg_offset, fontDims);
			u32 strW = ArrayCount("ammo") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			printString(renderer, "ammo", ArrayCount("ammo"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 1)
		{
			printString(renderer, "100", ArrayCount("100"), font, ArrayCount(font), hudP + bg_offset, fontDims);
			u32 strW = ArrayCount("health") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			printString(renderer, "health", ArrayCount("health"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 2)
		{
			u32 strW = ArrayCount("arms") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			printString(renderer, "arms", ArrayCount("arms"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
		else if (i == 4)
		{
			printString(renderer, "100", ArrayCount("100"), font, ArrayCount(font), hudP + bg_offset, fontDims);
			u32 strW = ArrayCount("armor") * fontDims.x;
			u32 strHalf = RoundReal32ToUInt32(strW * 0.5f);
			printString(renderer, "armor", ArrayCount("armor"), font, ArrayCount(font), hudP + text_offset, fontDims);
		}
	}

	// Present buffers to the screen
	renderer.PresentBuffer();

	return Quit;
}

void DemoGame::UnloadContent()
{

}