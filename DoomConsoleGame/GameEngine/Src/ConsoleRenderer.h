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
	i32 Width;
	i32 Height;
	wchar_t *Buffer;
	u16		*Attribs;
	r32		*DepthBuffer;
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
	ConsoleRenderer();
	~ConsoleRenderer();
	
	ScreenBuffer* GetRenderBuffers();
	void SetScreenBuffer(ScreenBuffer *_screen);
	// Draws a single pixel onto the screen buffer using the desired x, y position, type of pixel and color.
	void DrawPixel(i32 x, i32 y, wchar_t pixel = PIXEL_SOLID, u16 color = PIXEL_COLOR_WHITE);
	// Draws a rectangle on the buffer
	void DrawRect(i32 x, i32 y, i32 w, i32 h, wchar_t pixel = PIXEL_SOLID, u16 color = PIXEL_COLOR_WHITE);
	// Draws a sprite onto the screen buffer using desired x, y pos and color.
	void DrawSprite(i32 posX, i32 posY, i32 spriteW, i32 spriteH, wchar_t* spriteData, u16 *colorData);
	// Clears the entire screen buffer to the desired color.
	void ClearBuffer(u16 color = PIXEL_COLOR_BLACK);
	// Draws the entire buffer to the screen
	void PresentBuffer();

protected:
	ScreenBuffer *screen;

};

#endif
