#include "DemoGame.h"
#include "platform.h"
#include "File.h"

void DemoGame::LoadContent()
{
	playerP.x = 1;
	playerP.y = 1;
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
	int pillarHandle = platform_fileOpen("data/pillar_01.sprt", "rb");
	FileReadData pillarData = platform_fileReadEntire(pillarHandle);
	platform_fileClose(pillarHandle);
	SpriteHeader* pillarHeader = (SpriteHeader*)pillarData.Data;
	if (pillarHeader)
	{
		if (pillarHeader->Sentinal[0] == 'S' && pillarHeader->Sentinal[1] == 'P' && pillarHeader->Sentinal[2] == 'R' && pillarHeader->Sentinal[3] == 'T')
		{
			pillar.Width = pillarHeader->Width;
			pillar.Height = pillarHeader->Height;
			u16* colors = (u16*)((u8*)pillarData.Data + pillarHeader->ColorOffset);
			wchar_t* pixels = (wchar_t*)((u8*)pillarData.Data + pillarHeader->PixelOffset);
			pillar.Colors = CreateArray(Memory::GetPersistantHandle(), u16, pillar.Width * pillar.Height);
			pillar.Pixels = CreateArray(Memory::GetPersistantHandle(), wchar_t, pillar.Width * pillar.Height);

			memcpy_s(pillar.Colors, pillar.Width*pillar.Height * sizeof(u16), colors, pillar.Width*pillar.Height * sizeof(u16));
			memcpy_s(pillar.Pixels, pillar.Width*pillar.Height * sizeof(wchar_t), pixels, pillar.Width*pillar.Height * sizeof(wchar_t));

			if(pillarData.Data)
				free(pillarData.Data);
		}
	}
	
	pillarP.x = 27.5f;
	pillarP.y = 5.5f;
	
	// Load the wall texture
	int wallHandle = platform_fileOpen("data/wall_01.sprt", "rb");
	FileReadData wallData = platform_fileReadEntire(wallHandle);
	platform_fileClose(wallHandle);
	SpriteHeader* wallHeader = (SpriteHeader*)wallData.Data;
	if (wallHeader)
	{
		if (wallHeader->Sentinal[0] == 'S' && wallHeader->Sentinal[1] == 'P' && wallHeader->Sentinal[2] == 'R' && wallHeader->Sentinal[3] == 'T')
		{
			wall.Width = wallHeader->Width;
			wall.Height = wallHeader->Height;
			u16* colors = (u16*)((u8*)wallData.Data + wallHeader->ColorOffset);
			wchar_t* pixels = (wchar_t*)((u8*)wallData.Data + wallHeader->PixelOffset);
			wall.Colors = CreateArray(Memory::GetPersistantHandle(), u16, wall.Width * wall.Height);
			wall.Pixels = CreateArray(Memory::GetPersistantHandle(), wchar_t, wall.Width * wall.Height);

			memcpy_s(wall.Colors, wall.Width*wall.Height * sizeof(u16), colors, wall.Width*wall.Height * sizeof(u16));
			memcpy_s(wall.Pixels, wall.Width*wall.Height * sizeof(wchar_t), pixels, wall.Width*wall.Height * sizeof(wchar_t));

			if (wallData.Data)
				free(wallData.Data);
		}
	}

	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			if (map[y * mapW + x] == 'S')
			{
				playerP.x = x;
				playerP.y = y;
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
		int DistanceToCeiling = (((float)screenHeight / 2) - ((float)screenHeight / DistanceToWall));
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

					i32 sampleIndexX = sampleX * wall.Width;
					i32 sampleIndexY = sampleY * wall.Height;

					sampleIndexX = Clamp(0, sampleIndexX, wall.Width - 1);
					sampleIndexY = Clamp(0, sampleIndexY, wall.Height - 1);

					Color = wall.Colors[sampleIndexY * wall.Width + sampleIndexX];
				}
				if (HitDoor)
				{

				}
				renderer.DrawPixel({ (r32)x, (r32)y }, shade, Color);
			}
			else
			{
				// Shade floor based on distance
				float b = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
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
	// We want to calculate the distance between the object and the player
	float directionToObjX = pillarP.x - playerP.x;
	float directionToObjY = pillarP.y - playerP.y;
	float distanceToPlayer = sqrtf(directionToObjX*directionToObjX + directionToObjY*directionToObjY);

	// Then we want to create our forward vector
	// Calculate the objects angle between the forward vector and direction that the object is in.
	vec2 eye = {};
	eye.x = sinf(playerAngle);
	eye.y = cosf(playerAngle);
	float objectAngle = atan2f(eye.y, eye.x) - atan2f(directionToObjY, directionToObjX);
	if (objectAngle < -PI)
		objectAngle += 2.0f * PI;
	if (objectAngle > PI)
		objectAngle -= 2.0f * PI;

	// Here we check to see if the angle is with in the view.
	bool ObjectInView = fabs(objectAngle) < FOV / 2.0f;

	// If the object is in view and the distance from the object to the player is with in a certain range
	if (ObjectInView && distanceToPlayer >= 0.5f && distanceToPlayer < depth)
	{
		// Then we want to calculate the objects dimensions in world space.
		// First we calculate where the objects ceiling starts based on the distance
		// Then the floor location
		// this will give us how tall the object will look with in our view. The farther the smaller it will be. The closer the bigger it will be.
		float ObjectCeiling = (float)(screenHeight / 2.0f) - screenHeight / distanceToPlayer;
		float ObjectFloor = screenHeight - ObjectCeiling;
		float ObjectHeight = ObjectFloor - ObjectCeiling;
		float ObjectAspectRatio = (float)pillar.Height / (float)pillar.Width;
		float ObjectWidth = ObjectHeight / ObjectAspectRatio;
		float ObjectCenter = (0.5f * (objectAngle / (FOV / 2.0f)) + 0.5f) * (float)screenWidth;

		for (int y = 0; y < ObjectHeight; ++y)
		{
			for (int x = 0; x < ObjectWidth; ++x)
			{
				float SampleX = x / ObjectWidth;
				float SampleY = y / ObjectHeight;
				int Row = SampleY * pillar.Height;
				int Col = SampleX * pillar.Width;
				wchar_t glyph = pillar.Pixels[Row * pillar.Width + Col];
				u16 color = pillar.Colors[Row * pillar.Width + Col];
				int ObjectCol = (int)(ObjectCenter + x - (ObjectWidth / 2.0f));
				if (ObjectCol >= 0 && ObjectCol < screenWidth)
				{
					if (glyph != ' ' && renderer.GetRenderBuffers()->DepthBuffer[ObjectCol] >= distanceToPlayer)
						renderer.DrawPixel({(r32)ObjectCol, (r32)ObjectCeiling + y}, glyph, color);
				}
			}
		}
	}

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

	// Present buffers to the screen
	renderer.PresentBuffer();

	return Quit;
}

void DemoGame::UnloadContent()
{

}