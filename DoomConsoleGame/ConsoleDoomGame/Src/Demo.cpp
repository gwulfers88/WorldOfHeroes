/*******************************************************
FileName: Entry.cpp
StartDate: 01/17/2018
ModDate: 01/18/2018
CreatorName: George Wulfers
ContributorsName: George Wulfers
===================================================
This is the entry point to our progrqam.
*******************************************************/

#pragma comment(lib, "GameEngine_Debug.lib")

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <Windows.h>
#include "memory.h"
#include "ConsoleRenderer.h"

#ifdef _DEBUG
#define Assert(Exp) { if(!Exp) { *(int *)0 = 0; } }
#elif
#define Assert(Exp)
#endif // DEBUG

#define ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#define Kilobytes(Size) (Size * 1024)
#define Megabytes(Size) (Kilobytes(Size) * 1024)
#define Gigabytes(Size) (Megabytes(Size) * 1024)

#define PI 3.14159265359f

//int argCount -- Used to determine the number of arguments being passed to the application
//char** args  -- Array list of arguments entered.
int main(int argCount, char** args)
{
	for (int i = 0; i < argCount; ++i)
	{
		printf("argCount: %d -- args: %s \n", i, args[i]);
	}

	// Initialize our memory pools.
	size_t BaseSize = Megabytes(500);
	InitializeMemory(BaseSize);
	MemoryHandle PersistantHandle = InitializeChunk((int)(BaseSize * 0.5f));
	MemoryHandle TransientHandle = InitializeChunk((int)(BaseSize * 0.5f));

	// Create our buffers
	screenBuffer = CreateArray(PersistantHandle, wchar_t, screenWidth*screenHeight);	// used for text
	screenAttribs = CreateArray(PersistantHandle, WORD, screenWidth*screenHeight);		// used to color text
	depthBuffer = CreateArray(TransientHandle, float, screenWidth);

	// Check to see if we were able to allocate memory from our pool.
	Assert(screenBuffer);
	Assert(screenAttribs);
	Assert(depthBuffer);

	// Get handle to a new console window that we can read and write from.
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);

	// Set initial window size to the smalled possible. this is needed to be able to set a propper size for the window.
	SMALL_RECT rect = { 0, 0, 1, 1 };
	Assert(SetConsoleWindowInfo(hConsole, TRUE, &rect));

	// Set the desired buffer size for the window.
	Assert(SetConsoleScreenBufferSize(hConsole, { (short)screenWidth, (short)screenHeight }));

	// Let windows know that we want to use our new window.
	Assert(SetConsoleActiveScreenBuffer(hConsole));

	// Set the font size now that the screen buffer has been assigned to the console
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");

	Assert(SetCurrentConsoleFontEx(hConsole, false, &cfi));

	// Check to see if the current buffer is the correct size for our window.
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	if (screenWidth > consoleInfo.dwMaximumWindowSize.X)
	{
		MessageBox(0, L"Width is greater than max Width\n", L"ERROR!", MB_OK);
	}
	if (screenHeight > consoleInfo.dwMaximumWindowSize.Y)
	{
		MessageBox(0, L"Height is greater than max Height\n", L"ERROR!", MB_OK);
	}

	// set the actual window size.
	rect = { 0, 0, (short)screenWidth - 1, (short)screenHeight - 1 };
	Assert(SetConsoleWindowInfo(hConsole, TRUE, &rect));

	// Let windows know that we want to handle some events like input and mouse input.
	SetConsoleMode(hConsole, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	// set title for window.
	SetConsoleTitleW(L"Console Game Engine");

	int mapW = 40;
	int mapH = 20;
	wchar_t map[] =
	{
		L"########################################"
		L"#......................................#"
		L"#..........#######.....................#"
		L"#..........#.....#.....................#"
		L"#......S...#.....#.....................#"
		L"############.....#.....................#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#.....................#"
		L".......................................#"
		L".......................................#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#.....................#"
		L".................#######################"
	};

	float playerX = 1;
	float playerY = 1;
	float playerAngle = 0;
	float FOV = PI / 4.0f;
	float depth = 40.0f;

	float deltaTime = 1.0f / 60.0f;

	float SpriteX = 27.5f;
	float SpriteY = 5.5f;
	int SpriteW = 14;
	int SpriteH = 10;
	wchar_t Sprite[] =
	{
		L"              "
		L"              "
		L"              "
		L"              "
		L"              "
		L"              "
		L"     ####     "
		L"     ####     "
		L"   ########   "
		L"  ##########  "
	};

	for (int y = 0; y < mapH; ++y)
	{
		for (int x = 0; x < mapW; ++x)
		{
			if (map[y * mapW + x] == 'S')
			{
				playerX = x;
				playerY = y;
			}
		}
	}

	// Game loop
	bool Quit = false;
	while (!Quit)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			Quit = true;

		float playerSpeed = 10.0f;

		// Forward / Backward Movement
		if (GetAsyncKeyState((unsigned short)L'W') & 0x8000)
		{
			playerX += sinf(playerAngle) * playerSpeed * deltaTime;
			playerY += cosf(playerAngle) * playerSpeed * deltaTime;

			if (map[(int)playerY * mapW + (int)playerX] == L'#')
			{
				playerX -= sinf(playerAngle) * playerSpeed * deltaTime;
				playerY -= cosf(playerAngle) * playerSpeed * deltaTime;
			}
		}
		else if (GetAsyncKeyState((unsigned short)L'S') & 0x8000)
		{
			playerX -= sinf(playerAngle) * playerSpeed * deltaTime;
			playerY -= cosf(playerAngle) * playerSpeed * deltaTime;

			if (map[(int)playerY * mapW + (int)playerX] == L'#')
			{
				playerX += sinf(playerAngle) * playerSpeed * deltaTime;
				playerY += cosf(playerAngle) * playerSpeed * deltaTime;
			}
		}

		// Roatting the Angle CCW / CW
		if (GetAsyncKeyState((unsigned short)L'A') & 0x8000)
		{
			playerAngle -= (playerSpeed * 0.75f) * deltaTime;
		}
		else if (GetAsyncKeyState((unsigned short)L'D') & 0x8000)
		{
			playerAngle += (playerSpeed * 0.75f) * deltaTime;
		}

		// Straffe Movement
		if (GetAsyncKeyState((unsigned short)L'Q') & 0x8000)
		{
			playerX -= cosf(playerAngle) * playerSpeed * deltaTime;
			playerY += sinf(playerAngle) * playerSpeed * deltaTime;

			if (map[(int)playerY * mapW + (int)playerX] == L'#')
			{
				playerX += cosf(playerAngle) * playerSpeed * deltaTime;
				playerY -= sinf(playerAngle) * playerSpeed * deltaTime;
			}
		}
		else if (GetAsyncKeyState((unsigned short)L'E') & 0x8000)
		{
			playerX += cosf(playerAngle) * playerSpeed * deltaTime;
			playerY -= sinf(playerAngle) * playerSpeed * deltaTime;

			if (map[(int)playerY * mapW + (int)playerX] == L'#')
			{
				playerX -= cosf(playerAngle) * playerSpeed * deltaTime;
				playerY += sinf(playerAngle) * playerSpeed * deltaTime;
			}
		}

		// Clear buffer to certain color
		//ClearBuffer(PIXEL_COLOR_GREY);

		// Ray Casting Routine
		for (int x = 0; x < screenWidth; ++x)
		{
			// Find the ray angle based on the players Rotattion angle
			float RayAngle = (playerAngle - FOV / 2) + ((float)x / screenWidth) * FOV;

			float StepSize = 0.01f;			// This is how far we will advanced the ray per iteration
			float DistanceToWall = 0.0f;	// 

			bool HitWall = false;
			bool Boundary = false;

			float EyeX = sinf(RayAngle);
			float EyeY = cosf(RayAngle);

			while (!HitWall && DistanceToWall < depth)
			{
				DistanceToWall += StepSize;

				int TestX = (int)(playerX + EyeX * DistanceToWall);
				int TestY = (int)(playerY + EyeY * DistanceToWall);

				// Our ray has gone Out of bounds
				if (TestX < 0 || TestX >= mapW || TestY < 0 || TestY >= mapH)
				{
					HitWall = true;
					DistanceToWall = depth;
				}
				else
				{
					// Ray is still in bounds to test cell for walls
					if (map[TestY * mapW + TestX] == L'#')
					{
						// Ray has hit a wall
						HitWall = true;

						// Calculate Boundaries of walls
						// To highlight tile boundaries, cast a ray from each corner
						// of the tile, to the player. The more coincident this ray
						// is to the rendering ray, the closer we are to a tile 
						// boundary, which we'll shade to add detail to the walls
						std::vector<std::pair<float, float>> p;

						// Test each corner of hit tile, storing the distance from
						// the player, and the calculated dot product of the two rays
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								// Angle of corner to eye
								float vy = (float)TestY + ty - playerY;
								float vx = (float)TestX + tx - playerX;
								float d = sqrt(vx*vx + vy*vy);
								float dot = (EyeX * vx / d) + (EyeY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right) {return left.first < right.first; });

						// First two/three are closest (we will never see all four)
						float Bound = 0.01;
						if (acos(p.at(0).second) < Bound) Boundary = true;
						if (acos(p.at(1).second) < Bound) Boundary = true;
						//if (acos(p.at(2).second) < Bound) Boundary = true;
					}
				}
			}

			// Calculate the distance to the ceiling
			int DistanceToCeiling = ((float)screenHeight / 2) - (float)screenHeight / DistanceToWall;
			int DistanceToFloor = screenHeight - DistanceToCeiling;

			depthBuffer[x] = DistanceToWall;

			// Shade walls based on distance 
			wchar_t shade = PIXEL_SOLID;
			short Color = PIXEL_COLOR_WHITE;

			if (DistanceToWall <= depth / 4)
			{
				shade = PIXEL_SOLID;
				Color = PIXEL_COLOR_WHITE;
			}
			else if (DistanceToWall < depth / 3)
			{
				shade = PIXEL_SEMI_DARK;
				Color = PIXEL_COLOR_WHITE;
			}
			else if (DistanceToWall < depth / 2)
			{
				shade = PIXEL_MEDIUM_DARK;
				Color = PIXEL_COLOR_GREY;
			}
			else
			{
				shade = PIXEL_DARK;
				Color = PIXEL_COLOR_BLACK;
			}

			if (Boundary)
			{
				shade = PIXEL_DARK;
				Color = PIXEL_COLOR_BLACK;
			}

			for (int y = 0; y < screenHeight; ++y)
			{
				if (y <= DistanceToCeiling)
				{
					DrawPixel(x, y, PIXEL_SOLID, PIXEL_COLOR_BLACK);
				}
				else if (y > DistanceToCeiling && y <= DistanceToFloor)
				{
					DrawPixel(x, y, shade, Color);
				}
				else
				{
					// Shade floor based on distance
					float b = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
					if (b < 0.25)
					{
						shade = PIXEL_SOLID;
						Color = PIXEL_COLOR_GREY;
					}
					else if (b < 0.5)
					{
						shade = PIXEL_SEMI_DARK;
						Color = PIXEL_COLOR_GREY;
					}
					else if (b < 0.75)
					{
						shade = PIXEL_MEDIUM_DARK;
						Color = PIXEL_COLOR_GREY;
					}
					else if (b < 0.9)
					{
						shade = PIXEL_DARK;
						Color = PIXEL_COLOR_GREY;
					}
					DrawPixel(x, y, shade, Color);
				}
			}
		}

		// drawing Objects
		// We want to calculate the distance between the object and the player
		float directionToObjX = SpriteX - playerX;
		float directionToObjY = SpriteY - playerY;
		float distanceToPlayer = sqrtf(directionToObjX*directionToObjX + directionToObjY*directionToObjY);

		// Then we want to create our forward vector
		// Calculate the objects angle between the forward vector and direction that the object is in.
		float EyeX = sinf(playerAngle);
		float EyeY = cosf(playerAngle);
		float objectAngle = atan2f(EyeY, EyeX) - atan2f(directionToObjY, directionToObjX);
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
			float ObjectAspectRatio = (float)SpriteH / (float)SpriteW;
			float ObjectWidth = ObjectHeight / ObjectAspectRatio;
			float ObjectCenter = (0.5f * (objectAngle / (FOV / 2.0f)) + 0.5f) * (float)screenWidth;

			for (int y = 0; y < ObjectHeight; ++y)
			{
				for (int x = 0; x < ObjectWidth; ++x)
				{
					float SampleX = x / ObjectWidth;
					float SampleY = y / ObjectHeight;
					int Row = SampleY * SpriteH;
					int Col = SampleX * SpriteW;
					wchar_t glyph = Sprite[Row * SpriteW + Col];

					int ObjectCol = (int)(ObjectCenter + x - (ObjectWidth / 2.0f));
					if (ObjectCol >= 0 && ObjectCol < screenWidth)
					{
						if (glyph == '#' && depthBuffer[ObjectCol] >= distanceToPlayer)
							DrawPixel(ObjectCol, ObjectCeiling + y, PIXEL_SOLID, PIXEL_COLOR_DARK_RED);
					}
				}
			}
		}

		// 2D Map
		int Screen1X = 10;
		int Screen1Y = 10;

		// Draw Map
		for (int y = 0; y < mapH; ++y)
		{
			for (int x = 0; x < mapW; ++x)
			{
				if (map[y * mapW + x] == '#')
				{
					DrawPixel(Screen1X + x, Screen1Y + y, PIXEL_SOLID, PIXEL_COLOR_DARK_RED); // Walls
				}
				else
					DrawPixel(Screen1X + x, Screen1Y + y, PIXEL_SOLID, PIXEL_COLOR_GREY); // Floor
			}
		}

		// This will draw the players field of view on the 2D map.
		for (int x = 0; x < mapW; ++x)
		{
			float ViewAngle = (playerAngle - FOV / 2) + ((float)x / mapW) * FOV;
			float EyeX = sinf(ViewAngle);
			float EyeY = cosf(ViewAngle);

			for (int y = 0; y < mapH; ++y)
			{
				DrawPixel((int)(playerX + Screen1X + EyeX * y), (int)(playerY + Screen1Y + EyeY * y), PIXEL_SOLID, PIXEL_COLOR_LIGHT_BLUE);
			}
		}
		// Draw player
		DrawPixel((int)(playerX + Screen1X), (int)(playerY + Screen1Y), PIXEL_SOLID, PIXEL_COLOR_LIGHT_GREEN);

		DrawPixel((int)(SpriteX + Screen1X), (int)(SpriteY + Screen1Y), PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_RED);

		// Present buffers to the screen
		PresentBuffer(hConsole);
	}

	CloseHandle(hConsole);
	FreeMemory();

	return 0;
}