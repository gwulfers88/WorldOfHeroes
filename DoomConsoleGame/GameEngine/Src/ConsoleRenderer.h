/*******************************************************
FileName: ConsoleRenderer.h
StartDate: 01/22/2018
ModDate: 
CreatorName: George Wulfers
ContributorsName: 
===================================================
Collection of Console commands and functions to help 
render the current game
*******************************************************/

#ifndef CRenderer_H
#define CRenderer_H

struct ScreenBuffer
{
	int Width;
	int Height;
};

int screenWidth = 320; //160;
int screenHeight = 176; //88;
wchar_t* screenBuffer = 0;
unsigned short* screenAttribs = 0;
float *depthBuffer = 0;

int fontw = 4;// 8;
int fonth = 4;// 8;

// Types of Pixels to use
#define PIXEL_SOLID 0x2588
#define PIXEL_DARK 0x2591
#define PIXEL_MEDIUM_DARK 0x2592
#define PIXEL_SEMI_DARK 0x2593

// All of the colors we can use
#define PIXEL_COLOR_INTENSITY		0x0008
#define PIXEL_COLOR_BLACK			0x0
#define PIXEL_COLOR_DARK_RED		0x0004
#define PIXEL_COLOR_DARK_GREEN		0x0002
#define PIXEL_COLOR_DARK_BLUE		0x0001
#define PIXEL_COLOR_DARK_YELLOW		PIXEL_COLOR_DARK_RED | PIXEL_COLOR_DARK_GREEN
#define PIXEL_COLOR_DARK_MAGENTA	PIXEL_COLOR_DARK_RED | PIXEL_COLOR_DARK_BLUE
#define PIXEL_COLOR_DARK_CYAN		PIXEL_COLOR_DARK_GREEN | PIXEL_COLOR_DARK_BLUE
#define PIXEL_COLOR_LIGHT_RED		PIXEL_COLOR_DARK_RED | PIXEL_COLOR_INTENSITY
#define PIXEL_COLOR_LIGHT_GREEN		PIXEL_COLOR_DARK_GREEN | PIXEL_COLOR_INTENSITY
#define PIXEL_COLOR_LIGHT_BLUE		PIXEL_COLOR_DARK_BLUE | PIXEL_COLOR_INTENSITY
#define PIXEL_COLOR_LIGHT_YELLOW	PIXEL_COLOR_DARK_RED | PIXEL_COLOR_DARK_GREEN | PIXEL_COLOR_INTENSITY
#define PIXEL_COLOR_LIGHT_MAGENTA	PIXEL_COLOR_DARK_RED | PIXEL_COLOR_DARK_BLUE | PIXEL_COLOR_INTENSITY
#define PIXEL_COLOR_LIGHT_CYAN		PIXEL_COLOR_DARK_GREEN | PIXEL_COLOR_DARK_BLUE | PIXEL_COLOR_INTENSITY
#define PIXEL_COLOR_GREY			PIXEL_COLOR_DARK_RED | PIXEL_COLOR_DARK_GREEN | PIXEL_COLOR_DARK_BLUE
#define PIXEL_COLOR_WHITE			PIXEL_COLOR_GREY | PIXEL_COLOR_INTENSITY

// Draws a single pixel onto the screen buffer using the desired x, y position, type of pixel and color.
void DrawPixel(int x, int y, wchar_t pixel = PIXEL_SOLID, unsigned short color = PIXEL_COLOR_WHITE)
{
	if (x < 0)
	{
		x = 0;
	}
	if (x >= screenWidth)
	{
		x = screenWidth - 1;
	}
	if (y < 0)
	{
		y = 0;
	}
	if (y >= screenHeight)
	{
		y = screenHeight - 1;
	}

	screenBuffer[y * screenWidth + x] = pixel;
	screenAttribs[y * screenWidth + x] = color;
}

// Draws a sprite onto the screen buffer using desired x, y pos and color.
void DrawSprite(int posX, int posY, int spriteW, int spriteH, wchar_t* spriteData, unsigned short color = PIXEL_COLOR_WHITE)
{
	for (int col = 0; col < spriteH; ++col)
	{
		for (int row = 0; row < spriteW; ++row)
		{
			int pixelX = posX + row;
			int pixelY = posY + col;
			wchar_t glyph = spriteData[col * spriteW + row];
			short c = (glyph == '#') ? color : PIXEL_COLOR_BLACK;
			DrawPixel(pixelX, pixelY, PIXEL_SOLID, c);
		}
	}
}

// Clears the entire screen buffer to the desired color.
void ClearBuffer(short color = PIXEL_COLOR_BLACK)
{
	for (int y = 0; y < screenHeight; ++y)
	{
		for (int x = 0; x < screenWidth; ++x)
		{
			DrawPixel(x, y, PIXEL_SOLID, color);
		}
	}
}

// Draws the entire buffer to the screen
void PresentBuffer(HANDLE handle)
{
	DWORD bytesWritten = 0;
	screenBuffer[screenWidth * screenHeight - 1] = '\0';
	WriteConsoleOutputCharacter(handle, screenBuffer, screenWidth * screenHeight, { 0, 0 }, &bytesWritten);
	WriteConsoleOutputAttribute(handle, screenAttribs, screenWidth * screenHeight, { 0, 0 }, &bytesWritten);
}

#endif
