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

#include "platform.h"

#pragma pack(push, 16)
struct ScreenBuffer
{
	int Width;
	int Height;
	wchar_t *Buffer;
	WORD	*Attribs;
	float	*DepthBuffer;
	HANDLE	consoleHandle;
};
#pragma pack(pop)

// Types of Pixels to use
#define PIXEL_SOLID			0x2588
#define PIXEL_DARK			0x2591
#define PIXEL_MEDIUM_DARK	0x2592
#define PIXEL_SEMI_DARK		0x2593

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

class ConsoleRenderer
{
public:
	ConsoleRenderer() {}
	~ConsoleRenderer() {}
	
	ScreenBuffer* GetRenderBuffers() { return screen; }

	void SetScreenBuffer(ScreenBuffer *_screen)
	{
		screen = _screen;
		Assert(screen);
		// Check to see if we were able to allocate memory from our pool.
		Assert(screen->Buffer);
		Assert(screen->Attribs);
		Assert(screen->DepthBuffer);
	}

	// Draws a single pixel onto the screen buffer using the desired x, y position, type of pixel and color.
	void DrawPixel(int x, int y, wchar_t pixel = PIXEL_SOLID, unsigned short color = PIXEL_COLOR_WHITE)
	{
		if (x < 0)
		{
			x = 0;
		}
		if (x >= screen->Width)
		{
			x = screen->Width - 1;
		}
		if (y < 0)
		{
			y = 0;
		}
		if (y >= screen->Height)
		{
			y = screen->Height - 1;
		}

		screen->Buffer[y * screen->Width + x] = pixel;
		screen->Attribs[y * screen->Width + x] = color;
	}

	// Draws a sprite onto the screen buffer using desired x, y pos and color.
	void DrawSprite(int posX, int posY, int spriteW, int spriteH, wchar_t* spriteData, unsigned short *colorData)
	{
		for (int col = 0; col < spriteH; ++col)
		{
			for (int row = 0; row < spriteW; ++row)
			{
				int pixelX = posX + row;
				int pixelY = posY + col;
				wchar_t glyph = spriteData[col * spriteW + row];
				unsigned short color = colorData[col * spriteW + row];
				DrawPixel(pixelX, pixelY, glyph, color);
			}
		}
	}

	// Clears the entire screen buffer to the desired color.
	void ClearBuffer(short color = PIXEL_COLOR_BLACK)
	{
		for (int y = 0; y < screen->Height; ++y)
		{
			for (int x = 0; x < screen->Width; ++x)
			{
				DrawPixel(x, y, PIXEL_SOLID, color);
			}
		}
	}

	// Draws the entire buffer to the screen
	void PresentBuffer()
	{
		DWORD bytesWritten = 0;
		screen->Buffer[screen->Width * screen->Height - 1] = '\0';
		WriteConsoleOutputCharacter(screen->consoleHandle, screen->Buffer, screen->Width * screen->Height, { 0, 0 }, &bytesWritten);
		WriteConsoleOutputAttribute(screen->consoleHandle, screen->Attribs, screen->Width * screen->Height, { 0, 0 }, &bytesWritten);
	}

protected:
	ScreenBuffer *screen;

};

#endif
