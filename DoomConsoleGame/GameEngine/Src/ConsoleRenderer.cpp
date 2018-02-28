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
void ConsoleRenderer::DrawPixel(vec2 pos, wchar_t pixel, u16 color)
{
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	if (pos.x >= screen->Width)
	{
		pos.x = screen->Width - 1;
	}
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	if (pos.y >= screen->Height)
	{
		pos.y = screen->Height - 1;
	}

	screen->Buffer[(int)pos.y * screen->Width + (int)pos.x] = pixel;
	screen->Attribs[(int)pos.y * screen->Width + (int)pos.x] = color;
}

void ConsoleRenderer::DrawRect(vec2 pos, vec2 dims, wchar_t pixel, u16 color)
{
	for (u32 row = pos.y; row < dims.y; row++)
	{
		for (u32 col = pos.x; col < dims.x; col++)
		{
			DrawPixel({(r32)col, (r32)row}, pixel, color);
		}
	}
}

// Draws a sprite onto the screen buffer using desired x, y pos and color.
void ConsoleRenderer::DrawSprite(vec2 pos, vec2 dims, wchar_t* spriteData, u16 *colorData)
{
	for (i32 col = 0; col < dims.y; ++col)
	{
		for (i32 row = 0; row < dims.x; ++row)
		{
			i32 pixelX = pos.x + row;
			i32 pixelY = pos.y + col;
			wchar_t glyph = spriteData[col * (int)dims.x + row];
			u16 color = colorData[col * (int)dims.x + row];
			DrawPixel({(r32)pixelX, (r32)pixelY}, glyph, color);
		}
	}
}

// Draws a sprite with the specified dimensions
void ConsoleRenderer::DrawUI(vec2 pos, vec2 dims, Sprite* img)
{
	for (i32 y = 0; y < dims.y; y++)
	{
		for (i32 x = 0; x < dims.x; x++)
		{
			// Calculate the texel coordinate for the rectangle
			vec2 sample = Vec2(x / dims.x, y / dims.y);
			// Sample the texture you want to use using the texel coords.
			u16 Color = SampleSprite(sample, img);
			// Draw Pixel
			DrawPixel({ pos.x + x, pos.y + y }, PIXEL_SOLID, Color);
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
			DrawPixel({(r32)x, (r32)y}, PIXEL_SOLID, color);
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