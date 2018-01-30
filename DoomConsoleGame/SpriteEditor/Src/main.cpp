/*
	main.cpp
	George Wulfers
	01/29/2018

	This is the entry point of the sprite editor 
*/
#pragma comment(lib, "GameEngine_Debug.lib")

#include <Windows.h>
#include "ConsoleRenderer.h"
#include "memory.h"

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

struct Sprite
{
	int Width;
	int Height;
	wchar_t* Pixels;
	WORD *Colors;
};

Sprite CreateSprite(int W, int H, MemoryHandle memory)
{
	Sprite sprite = {};
	size_t TotalSize = W * H;
	sprite.Pixels = CreateArray(memory, wchar_t, TotalSize);
	sprite.Colors = CreateArray(memory, WORD, TotalSize);

	Assert(sprite.Pixels);
	Assert(sprite.Colors);

	sprite.Width = W;
	sprite.Height = H;

	return sprite;
}

int main()
{
	// Initialize our memory pools.
	size_t BaseSize = Megabytes(500);
	InitializeMemory(BaseSize);
	MemoryHandle PersistantHandle = InitializeChunk((int)(BaseSize * 0.5f));
	MemoryHandle TransientHandle = InitializeChunk((int)(BaseSize * 0.5f));

	screenWidth = 120;
	screenHeight = 75;
	fontw = 10;
	fonth = 10;
	// Create our buffers
	screenBuffer = CreateArray(PersistantHandle, wchar_t, screenWidth*screenHeight);	// used for text
	screenAttribs = CreateArray(PersistantHandle, WORD, screenWidth*screenHeight);		// used to color text
	
	// Check to see if we were able to allocate memory from our pool.
	Assert(screenBuffer);
	Assert(screenAttribs);
	
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
	SetConsoleTitleW(L"Sprite Editor v1.0 - George Wulfers");

	Sprite sprite = CreateSprite(100, 50, PersistantHandle);
	
	int CanvasX = 10;
	int CanvasY = 10;

	float CursorX = 0;
	float CursorY = 0;
	float CursorSpeed = 10;

	float deltaTime = 1.0f / 60.0f;

	bool Quit = false;
	while (!Quit)
	{
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			Quit = true;

		if (GetAsyncKeyState('W') & 0x8000)
		{
			CursorY -= CursorSpeed * deltaTime;
		}
		else if (GetAsyncKeyState('S') & 0x8000)
		{
			CursorY += CursorSpeed * deltaTime;
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			CursorX -= CursorSpeed * deltaTime;
		}
		else if (GetAsyncKeyState('D') & 0x8000)
		{
			CursorX += CursorSpeed * deltaTime;
		}

		if (CursorX < 0)
			CursorX = 0;
		if (CursorX >= sprite.Width)
			CursorX = sprite.Width - 1;

		if (CursorY < 0)
			CursorY = 0;
		if (CursorY >= sprite.Height)
			CursorY = sprite.Height - 1;

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			int x = (int)CursorX;
			int y = (int)CursorY;

			sprite.Pixels[y * sprite.Width + x] = '#';
		}
		else if (GetAsyncKeyState(VK_DELETE) & 0x8000)
		{
			int x = (int)CursorX;
			int y = (int)CursorY;

			sprite.Pixels[y * sprite.Width + x] = ' ';
		}

		ClearBuffer(PIXEL_COLOR_GREY);

		// Draw Canvas
		for (int y = 0; y < sprite.Height + 1; ++y)
		{
			for (int x = 0; x < sprite.Width + 1; ++x)
			{
				// Top and Bottom Lines
				if (y == 0)
					DrawPixel(x + (CanvasX - 1), y + (CanvasY - 1), PIXEL_SOLID, PIXEL_COLOR_WHITE);
				if (y == sprite.Height)
					DrawPixel(x + (CanvasX), y + (CanvasY), PIXEL_SOLID, PIXEL_COLOR_WHITE);

				if (x == 0)
					DrawPixel(x + (CanvasX - 1), y + (CanvasY - 1), PIXEL_SOLID, PIXEL_COLOR_WHITE);
				if (x == sprite.Width)
					DrawPixel(x + (CanvasX), y + (CanvasY), PIXEL_SOLID, PIXEL_COLOR_WHITE);
			}
		}

		DrawSprite(CanvasX, CanvasY, sprite.Width, sprite.Height, sprite.Pixels, PIXEL_COLOR_WHITE);

		// Draw Cursor
		DrawPixel(CanvasX + (int)CursorX, CanvasY + (int)CursorY, PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_CYAN);


		PresentBuffer(hConsole);
	}
	
	FreeConsole();
	FreeMemory();

	return 0;
}