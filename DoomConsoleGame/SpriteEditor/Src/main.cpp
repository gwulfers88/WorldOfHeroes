/*
	main.cpp
	George Wulfers
	01/29/2018

	This is the entry point of the sprite editor 
*/
#pragma comment(lib, "GameEngine_Debug.lib")

#include "platform.h"
#include "Sprite.h"
#include "ConsoleRenderer.h"
#include "memory.h"

MemoryHandle PersistantHandle = 0;
MemoryHandle TransientHandle = 0;

char loadFilename[] = "test";
char saveFilename[] = "new_image";
char Path[] = "Save/";
char fileExt[] = ".sprt";

struct FileRead
{
	void* Data;
	long fileSize;
};

long GetFileSize(FILE* file)
{
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	return fileSize;
}

FILE* image = 0;

void WriteFile(Sprite sprite, char* filename)
{
	size_t totalSize = strlen(Path) + strlen(filename) + strlen(fileExt);
	char* buffer = CreateArray(PersistantHandle, char, totalSize);
	strcat(buffer, Path);
	strcat(buffer, filename);
	strcat(buffer, fileExt);

	fopen_s(&image, buffer, "wb");
	Assert(image);

	size_t TotalPixelSize = GetPixelSize(sprite);
	size_t TotalColorSize = GetColorSize(sprite);

	SpriteHeader header = GetSpriteHeader(sprite);

	fwrite(&header, sizeof(SpriteHeader), 1, image);
	fwrite(sprite.Pixels, TotalPixelSize, 1, image);
	fwrite(sprite.Colors, TotalColorSize, 1, image);

	fclose(image);

	PullArray(PersistantHandle, char, totalSize);
}

FileRead ReadEntireFile(char* filename)
{
	FileRead fileRead = {};

	FILE* file = fopen(filename, "rb");
	long fileSize = GetFileSize(file);
	fileRead.fileSize = fileSize;
	fileRead.Data = CreateVoidPtr(PersistantHandle, void, fileSize);

	Assert(fileRead.Data);
	fread(fileRead.Data, fileSize, 1, file);
	fclose(file);

	return fileRead;
}

Sprite LoadSprite(char* filename)
{
	size_t totalSize = strlen(Path) + strlen(filename) + strlen(fileExt);
	char* buffer = CreateArray(PersistantHandle, char, totalSize);
	strcat(buffer, Path);
	strcat(buffer, filename);
	strcat(buffer, fileExt);

	Sprite sprite = {};
	FileRead fileRead = ReadEntireFile(buffer);	// Read the entire file into a buffer

	SpriteHeader* header = (SpriteHeader*)fileRead.Data;	// Fetch the header file
	Assert(header);	// make sure that we actually got a header 
	
	// Make sure that the file identifier is set properly other wise this is not a 
	// SPRT image
	if (header->Sentinal[0] == 'G' && header->Sentinal[1] == 'W')
	{
		Assert(header->FileSize == fileRead.fileSize);	// Make sure that the size of the file matches the headers file size
		sprite = CreateSprite(header->Width, header->Height, PersistantHandle);	// Create sprite object
		size_t PixelSize = GetPixelSize(sprite);	//Calculate sizes for Pixel array and Color array
		size_t ColorSize = GetColorSize(sprite);

		sprite.Pixels = (wchar_t*)((char*)fileRead.Data + header->PixelOffset);			// Fetching Pixel data
		sprite.Colors = (unsigned short*)((char*)fileRead.Data + header->ColorOffset);	// Fetching Color data
	}
	else	// This file is not in our SPRT format
	{
		MessageBox(0, L"Incorrect file format.", L"ERROR:", MB_ICONERROR | MB_OK);
	}
	
	PullArray(PersistantHandle, char, totalSize);	// Release the string buffer we allocated earlier
	
	return sprite;
}

int main()
{
	// Initialize our memory pools.
	size_t BaseSize = Megabytes(50);
	InitializeMemory(BaseSize);
	PersistantHandle = InitializeChunk((int)(BaseSize * 0.5f));
	TransientHandle = InitializeChunk((int)(BaseSize * 0.5f));

	screenWidth = 400;
	screenHeight = 200;
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

retry:
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
		//MessageBox(0, L"Width is greater than max Width\n", L"ERROR!", MB_OK);
		screenWidth = consoleInfo.dwMaximumWindowSize.X / 2;
		goto retry;
	}
	if (screenHeight > consoleInfo.dwMaximumWindowSize.Y)
	{
		//MessageBox(0, L"Height is greater than max Height\n", L"ERROR!", MB_OK);
		screenHeight = consoleInfo.dwMaximumWindowSize.Y / 2;
		goto retry;
	}

	// set the actual window size.
	rect = { 0, 0, (short)screenWidth - 1, (short)screenHeight - 1 };
	Assert(SetConsoleWindowInfo(hConsole, TRUE, &rect));

	// Let windows know that we want to handle some events like input and mouse input.
	SetConsoleMode(hConsole, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	// set title for window.
	SetConsoleTitleW(L"Sprite Editor v1.0 - George Wulfers");

	Sprite sprite = {};
	if(ArrayCount(loadFilename) > 4)
		sprite = LoadSprite(loadFilename);
	else
		sprite = CreateSprite(screenWidth / 2, screenHeight / 2, PersistantHandle);

	int CanvasX = (int)((screenWidth/2.0f) - (sprite.Width/2.0f));
	int CanvasY = (int)((screenHeight / 2.0f) - (sprite.Height / 2.0f));

	float CursorX = 0;
	float CursorY = 0;
	float CursorSpeed = 5;
	float CursorRadius = 2.0f;

	float deltaTime = 1.0f / 60.0f;

	unsigned short Colors[] =
	{
		PIXEL_COLOR_BLACK,
		PIXEL_COLOR_DARK_RED,
		PIXEL_COLOR_DARK_GREEN,
		PIXEL_COLOR_DARK_BLUE,
		PIXEL_COLOR_DARK_YELLOW,
		PIXEL_COLOR_DARK_MAGENTA,
		PIXEL_COLOR_DARK_CYAN,
		PIXEL_COLOR_LIGHT_RED,
		PIXEL_COLOR_LIGHT_GREEN,
		PIXEL_COLOR_LIGHT_BLUE,
		PIXEL_COLOR_LIGHT_YELLOW,
		PIXEL_COLOR_LIGHT_MAGENTA,
		PIXEL_COLOR_LIGHT_CYAN,
		PIXEL_COLOR_GREY,
		PIXEL_COLOR_WHITE,
	};

	float colorID = (int)ArrayCount(Colors) - 1;

	int ColorsX = (int)((screenWidth / 2.0f) - ((colorID*2) / 2.0f));;
	int ColorsY = 5;

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
		if (CursorX + CursorRadius >= sprite.Width)
			CursorX = (int)sprite.Width - CursorRadius;

		if (CursorY < 0)
			CursorY = 0;
		if (CursorY + CursorRadius >= sprite.Height)
			CursorY = (int)sprite.Height - CursorRadius;

		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			colorID -= CursorSpeed * deltaTime;
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			colorID += CursorSpeed * deltaTime;
		}

		if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			CursorRadius += CursorSpeed * deltaTime;
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			CursorRadius -= CursorSpeed * deltaTime;
		}

		if (CursorRadius <= 0)
			CursorRadius = 1;
		if (CursorRadius >= 10)
			CursorRadius = 10;

		if (colorID < 0)
			colorID = 0;
		if (colorID >= ArrayCount(Colors))
		{
			colorID = (int)ArrayCount(Colors) - 1;
		}

		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			for (int y = 0; y < CursorRadius; ++y)
			{
				for (int x = 0; x < CursorRadius; ++x)
				{
					sprite.Pixels[(y + (int)CursorY) * sprite.Width + (x + (int)CursorX)] = PIXEL_SOLID;
					sprite.Colors[(y + (int)CursorY) * sprite.Width + (x + (int)CursorX)] = Colors[(int)colorID];
				}
			}
		}
		else if (GetAsyncKeyState(VK_DELETE) & 0x8000)
		{
			for (int y = 0; y < CursorRadius; ++y)
			{
				for (int x = 0; x < CursorRadius; ++x)
				{
					sprite.Pixels[(y + (int)CursorY) * sprite.Width + (x + (int)CursorX)] = ' ';
					sprite.Colors[(y + (int)CursorY) * sprite.Width + (x + (int)CursorX)] = 0;
				}
			}
		}

		ClearBuffer(PIXEL_COLOR_GREY);

		// Draw Color pallete
		for (int i = 0; i < ArrayCount(Colors); ++i)
		{
			if ((int)colorID == i)
			{
				DrawPixel(ColorsX + (i * 2) + 1, ColorsY, PIXEL_SOLID, PIXEL_COLOR_DARK_CYAN);
				DrawPixel(ColorsX + (i * 2) - 1, ColorsY, PIXEL_SOLID, PIXEL_COLOR_DARK_CYAN);
				DrawPixel(ColorsX + (i * 2), ColorsY + 1, PIXEL_SOLID, PIXEL_COLOR_DARK_CYAN);
				DrawPixel(ColorsX + (i * 2), ColorsY - 1, PIXEL_SOLID, PIXEL_COLOR_DARK_CYAN);
			}

			DrawPixel(ColorsX + (i * 2), ColorsY, PIXEL_SOLID, Colors[i]);
		}

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

		DrawSprite(CanvasX, CanvasY, sprite.Width, sprite.Height, sprite.Pixels, sprite.Colors);

		// Draw Cursor
		for (int y = 0; y < CursorRadius; ++y)
		{
			for (int x = 0; x < CursorRadius; ++x)
			{
				DrawPixel(CanvasX + (int)CursorX + (int)(x), CanvasY + (int)CursorY + (int)(y), PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_CYAN);
			}
		}
		
		PresentBuffer(hConsole);
	}
	
	if(ArrayCount(saveFilename) > 4)
		WriteFile(sprite, saveFilename);

	FreeConsole();
	FreeMemory();

	return 0;
}