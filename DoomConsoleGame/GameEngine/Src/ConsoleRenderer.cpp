#include "ConsoleRenderer.h"

ConsoleRenderer::ConsoleRenderer() {}
ConsoleRenderer::~ConsoleRenderer() {}

ScreenBuffer* ConsoleRenderer::GetRenderBuffers() { return screen; }

void ConsoleRenderer::SetScreenBuffer(ScreenBuffer *_screen)
{
	screen = _screen;
	Assert(screen);
	// Check to see if we were able to allocate memory from our pool.
	Assert(screen->Buffer);
	Assert(screen->Attribs);
	Assert(screen->DepthBuffer);
}

// Draws a single pixel onto the screen buffer using the desired x, y position, type of pixel and color.
void ConsoleRenderer::DrawPixel(i32 x, i32 y, wchar_t pixel, u16 color)
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

void ConsoleRenderer::DrawRect(i32 x, i32 y, i32 w, i32 h, wchar_t pixel, u16 color)
{
	for (u32 row = y; row < h; row++)
	{
		for (u32 col = x; col < w; col++)
		{
			DrawPixel(col, row, pixel, color);
		}
	}
}

// Draws a sprite onto the screen buffer using desired x, y pos and color.
void ConsoleRenderer::DrawSprite(i32 posX, i32 posY, i32 spriteW, i32 spriteH, wchar_t* spriteData, u16 *colorData)
{
	for (i32 col = 0; col < spriteH; ++col)
	{
		for (i32 row = 0; row < spriteW; ++row)
		{
			i32 pixelX = posX + row;
			i32 pixelY = posY + col;
			wchar_t glyph = spriteData[col * spriteW + row];
			u16 color = colorData[col * spriteW + row];
			DrawPixel(pixelX, pixelY, glyph, color);
		}
	}
}

// Clears the entire screen buffer to the desired color.
void ConsoleRenderer::ClearBuffer(u16 color)
{
	for (i32 y = 0; y < screen->Height; ++y)
	{
		for (i32 x = 0; x < screen->Width; ++x)
		{
			DrawPixel(x, y, PIXEL_SOLID, color);
		}
	}
}

// Draws the entire buffer to the screen
void ConsoleRenderer::PresentBuffer()
{
	DWORD bytesWritten = 0;
	screen->Buffer[screen->Width * screen->Height - 1] = '\0';
	WriteConsoleOutputCharacter(screen->consoleHandle, screen->Buffer, screen->Width * screen->Height, { 0, 0 }, &bytesWritten);
	WriteConsoleOutputAttribute(screen->consoleHandle, screen->Attribs, screen->Width * screen->Height, { 0, 0 }, &bytesWritten);
}