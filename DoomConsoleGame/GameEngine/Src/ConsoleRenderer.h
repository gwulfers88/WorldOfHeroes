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
#include "Sprite.h"
#include "vec2.h"
#include "Entity.h"

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

struct Camera
{
	vec2 Position;
	r32 rotation;
	r32 FOV;
	r32 Depth;
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
	void DrawPixel(vec2 pos, wchar_t pixel = PIXEL_SOLID, u16 color = PIXEL_COLOR_WHITE);
	// Draws a rectangle on the buffer
	void DrawRect(vec2 pos, vec2 dims, wchar_t pixel = PIXEL_SOLID, u16 color = PIXEL_COLOR_WHITE);
	// Draws a sprite onto the screen buffer using desired x, y pos and color.
	void DrawSprite(vec2 pos, vec2 dims, wchar_t* spriteData, u16 *colorData);
	// Projects 2D object into 3D World.
	void ProjectObject(Camera* camera, vec2 objP, Sprite* img);
	// Projects the world from 2D to 3D
	void ProjectWorld(Camera* camera, u32 MapW, u32 MapH, Sprite* wall);
	// This function prints strings using our font.
	void DrawString(char* text, u32 textCount, Sprite* font, u32 fontCount, vec2 pos, vec2 dims);

	// Draws a sprite with the specified dimensions
	void DrawUI(vec2 pos, vec2 dims, Sprite* img);
	// Clears the entire screen buffer to the desired color.
	void ClearBuffer(u16 color = PIXEL_COLOR_BLACK);
	// Draws the entire buffer to the screen
	void PresentBuffer();

protected:
	ScreenBuffer *screen;

};

#endif
