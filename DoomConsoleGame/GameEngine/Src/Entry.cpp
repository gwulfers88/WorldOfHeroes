/*******************************************************
	FileName: Entry.cpp
	StartDate: 01/17/2018
	ModDate: 01/18/2018
	CreatorName: George Wulfers
	ContributorsName: George Wulfers
	===================================================
	This is the entry point to our progrqam.
*******************************************************/

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <Windows.h>
#include "memory.h"
#include "ConsoleRenderer.h"

#define ArrayCount(Array) (sizeof(Array)/sizeof(Array[0]))
#define Assert(Exp) { if(!Exp) { *(int *)0 = 0; } }
#define Kilobytes(Size) (Size * 1024)
#define Megabytes(Size) (Kilobytes(Size) * 1024)
#define Gigabytes(Size) (Megabytes(Size) * 1024)

#define PI 3.14159265359f

#pragma pack(push, 2)
struct Obj
{
	int x;	//4
	int y;	//4
	char c;	//1
};
#pragma pack(pop)

class Base
{
public:
	~Base() {}
	void Method() {}
};

class Child : public Base
{
public:
	virtual ~Child() {}
	virtual void Method() {}
};

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

	// Check to see if we were able to allocate memory from our pool.
	Assert(screenBuffer);
	Assert(screenAttribs);

	// Get handle to a new console window that we can read and write from.
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);

	// Set initial window size to the smalled possible. this is needed to be able to set a propper size for the window.
	SMALL_RECT rect = {0, 0, 1, 1};
	Assert(SetConsoleWindowInfo(hConsole, TRUE, &rect));

	// Set the desired buffer size for the window.
	Assert(SetConsoleScreenBufferSize(hConsole, { (short)screenWidth, (short)screenHeight }));

	// Let windows know that we want to use our new window.
	Assert(SetConsoleActiveScreenBuffer(hConsole));

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
	rect = {0, 0, (short)screenWidth-1, (short)screenHeight-1};
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
		L"#..........######......................#"
		L"#......S...#...........................#"
		L"#..........#...........................#"
		L"############...........................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"#......................................#"
		L"########################################"
	};

	float playerX = 1;
	float playerY = 1;
	float playerAngle = 0;
	float FOV = PI / 4.0f;
	float depth = 20.0f;

	float deltaTime = 1.0f / 60.0f;

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

		float playerSpeed = 1.0f;

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
		ClearBuffer(PIXEL_COLOR_GREY);

		// 2D Map
		int Screen1X = (screenWidth / 2 - mapW / 2) - 20;
		int Screen1Y = screenHeight / 2 - mapH / 2;

		// 3D Map
		int Screen2X = (screenWidth / 2 + mapW / 2) - 20;
		int Screen2Y = screenHeight / 2 - mapH / 2;

		// Draw Map
		for (int y = 0; y < mapH; ++y)
		{
			for (int x = 0; x < mapW; ++x)
			{
				if (map[y * mapW + x] == '#')
				{
					DrawPixel(Screen1X + x, Screen1Y + y); // Walls
				}
				else
				{
					DrawPixel(Screen1X + x, Screen1Y + y, PIXEL_SOLID, PIXEL_COLOR_BLACK); // Nothing
				}
			}
		}

		// Ray Casting Routine
		for (int x = 0; x < mapW; ++x)
		{
			// Find the ray angle based on the players Rotattion angle
			float RayAngle = (playerAngle - FOV / 2) + ((float)x / mapW) * FOV;

			float StepSize = 0.1f;			// This is how far we will advanced the ray per iteration
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
						if (acos(p.at(2).second) < Bound) Boundary = true;
					}
				}
			}

			// Calculate the distance to the ceiling
			int DistanceToCeiling = ((float)mapH / 2) - (float)mapH / DistanceToWall;
			int DistanceToFloor = mapH - DistanceToCeiling;

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

			for (int y = 0; y < mapH; ++y)
			{
				if (y <= DistanceToCeiling)
				{
					DrawPixel(x + Screen2X, y + Screen2Y, PIXEL_SOLID, PIXEL_COLOR_BLACK);
				}
				else if ( y > DistanceToCeiling && y <= DistanceToFloor)
				{
					DrawPixel(x + Screen2X, y + Screen2Y, shade, Color);
				}
				else
				{
					// Shade floor based on distance
					float b = 1.0f - (((float)y - mapH / 2.0f) / ((float)mapH / 2.0f));
					if (b < 0.25)
					{
						shade = PIXEL_SOLID;
						Color = PIXEL_COLOR_WHITE;
					}
					else if (b < 0.5)
					{
						shade = PIXEL_SEMI_DARK;
						Color = PIXEL_COLOR_WHITE;
					}
					else if (b < 0.75)
					{
						shade = PIXEL_MEDIUM_DARK;
						Color = PIXEL_COLOR_GREY;
					}
					else if (b < 0.9) 
					{
						shade = PIXEL_DARK;
						Color = PIXEL_COLOR_BLACK;
					}
					DrawPixel(x + Screen2X, y + Screen2Y, shade, Color);
				}

				// This will draw the players field of view on the 2D map.
				DrawPixel((int)(playerX + Screen1X + EyeX * y), (int)(playerY + Screen1Y + EyeY * y), shade, PIXEL_COLOR_LIGHT_BLUE);
			}
		}

		// Draw player
		DrawPixel((int)(playerX + Screen1X), (int)(playerY + Screen1Y), PIXEL_SOLID, PIXEL_COLOR_LIGHT_GREEN);

		// Display Stats
		swprintf_s(&screenBuffer[0 * screenWidth + 40], 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f\0", playerX, playerY, playerAngle, 1.0f / deltaTime);
		swprintf_s(&screenBuffer[1 * screenWidth + 40], 40, L"Controls\0");
		swprintf_s(&screenBuffer[2 * screenWidth + 40], 40, L"W/S: Move Fwd / Bwd\0");
		swprintf_s(&screenBuffer[3 * screenWidth + 40], 40, L"A/D: Rotate\0");
		swprintf_s(&screenBuffer[4 * screenWidth + 40], 40, L"Q/E: Strafe\0");
		swprintf_s(&screenBuffer[5 * screenWidth + 40], 40, L"ESC: Exit\0");

		// Present buffers to the screen
		PresentBuffer(hConsole);
	}

	CloseHandle(hConsole);
	FreeMemory();

	return 0;
}