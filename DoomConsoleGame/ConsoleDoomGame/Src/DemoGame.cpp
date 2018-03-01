#include "DemoGame.h"
#include "platform.h"
#include "File.h"

void DemoGame::LoadContent()
{
	playerP = Vec2(1, 1);
	playerAngle = 0;
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
				playerP = Vec2(x, y);
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
			playerP.x += sinf(playerAngle) * playerSpeed * deltaTime;
			playerP.y += cosf(playerAngle) * playerSpeed * deltaTime;

			if (map[(int)playerP.y * mapW + (int)playerP.x] == L'#')
			{
				playerP.x -= sinf(playerAngle) * playerSpeed * deltaTime;
				playerP.y -= cosf(playerAngle) * playerSpeed * deltaTime;
			}
		}
		else if (Controller->MoveDown.pressed)
		{
			playerP.x -= sinf(playerAngle) * playerSpeed * deltaTime;
			playerP.y -= cosf(playerAngle) * playerSpeed * deltaTime;

			if (map[(int)playerP.y * mapW + (int)playerP.x] == L'#')
			{
				playerP.x += sinf(playerAngle) * playerSpeed * deltaTime;
				playerP.y += cosf(playerAngle) * playerSpeed * deltaTime;
			}
		}

		// Roatting the Angle CCW / CW
		if (Controller->MoveLeft.pressed)
		{
			playerAngle -= (playerSpeed * 0.75f) * deltaTime;
		}
		else if (Controller->MoveRight.pressed)
		{
			playerAngle += (playerSpeed * 0.75f) * deltaTime;
		}

		// Straffe Movement
		if (Controller->ActionLeft.pressed)
		{
			playerP.x -= cosf(playerAngle) * playerSpeed * deltaTime;
			playerP.y += sinf(playerAngle) * playerSpeed * deltaTime;

			if (map[(int)playerP.y * mapW + (int)playerP.x] == L'#')
			{
				playerP.x += cosf(playerAngle) * playerSpeed * deltaTime;
				playerP.y -= sinf(playerAngle) * playerSpeed * deltaTime;
			}
		}
		else if (Controller->ActionRight.pressed)
		{
			playerP.x += cosf(playerAngle) * playerSpeed * deltaTime;
			playerP.y -= sinf(playerAngle) * playerSpeed * deltaTime;

			if (map[(int)playerP.y * mapW + (int)playerP.x] == L'#')
			{
				playerP.x -= cosf(playerAngle) * playerSpeed * deltaTime;
				playerP.y += sinf(playerAngle) * playerSpeed * deltaTime;
			}
		}

	}

	// Clear buffer to certain color
	//ClearBuffer(PIXEL_COLOR_GREY);

	// Ray Casting Routine
	int screenWidth = renderer.GetRenderBuffers()->Width;
	int screenHeight = renderer.GetRenderBuffers()->Height;

	for (int x = 0; x < renderer.GetRenderBuffers()->Width; ++x)
	{
		// Find the ray angle based on the players Rotattion angle
		float RayAngle = (playerAngle - FOV / 2) + ((float)x / screenWidth) * FOV;

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

			i32 TestX = (playerP.x + eye.x * DistanceToWall);
			i32 TestY = (playerP.y + eye.y * DistanceToWall);

			r32 DX = (playerP.x + eye.x * DistanceToWall);
			r32 DY = (playerP.y + eye.y * DistanceToWall);

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
					vec2 testP = playerP + eye * DistanceToWall;
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
					r32 Dh = TestX + 0.5f;

					if (DX > Dh - 0.05f && DX < Dh + 0.05f)
					{
						// Calculate the Texture coordinates here.
						Color = PIXEL_COLOR_DARK_MAGENTA;

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
					Color = SampleSprite(Vec2(sampleX, sampleY), &wall);
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
	renderer.ProjectObject(playerP, playerAngle, FOV, depth, pillarP, &pillar);
	
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
		float ViewAngle = (playerAngle - FOV / 2) + ((float)x / mapW) * FOV;
		vec2 eye = {};
		eye.x = sinf(ViewAngle);
		eye.y = cosf(ViewAngle);

		for (int y = 0; y < mapH; ++y)
		{
			renderer.DrawPixel(playerP + screenOnePos + eye * y, PIXEL_SOLID, PIXEL_COLOR_LIGHT_BLUE);
		}
	}

	// Draw player
	renderer.DrawPixel(playerP + screenOnePos, PIXEL_SOLID, PIXEL_COLOR_LIGHT_GREEN);

	// Draw Pillar
	renderer.DrawPixel(pillarP + screenOnePos, PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_RED);

	for (int i = 0; i < 7; i++)
	{
		renderer.DrawUI(hudP + Vec2(i * (hudDims.x), 0), hudDims, &hudBG);
	}

	// Present buffers to the screen
	renderer.PresentBuffer();

	return Quit;
}

void DemoGame::UnloadContent()
{

}