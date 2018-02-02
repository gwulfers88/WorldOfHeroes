/*
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "memory.h"

#pragma pack(push, 16)
struct SpriteHeader
{
	wchar_t Sentinal[2];
	int Width;
	int Height;
	int FileSize;
	int PixelOffset;
	int ColorOffset;
};

struct Sprite
{
	int Width;
	int Height;
	wchar_t* Pixels;
	unsigned short *Colors;
};
#pragma pack(pop)

Sprite CreateSprite(MemoryHandle memory, int W, int H)
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

size_t GetPixelSize(Sprite sprite)
{
	size_t PixelSize = sprite.Width * sprite.Height * sizeof(wchar_t);
	return PixelSize;
}

size_t GetColorSize(Sprite sprite)
{
	size_t ColorSize = sprite.Width * sprite.Height * sizeof(unsigned short);
	return ColorSize;
}

SpriteHeader GetSpriteHeader(Sprite sprite)
{
	SpriteHeader header = {};

	size_t PixelSize = GetPixelSize(sprite);
	size_t ColorSize = GetColorSize(sprite);
	
	header.Sentinal[0] = 'G';
	header.Sentinal[1] = 'W';
	header.Width = sprite.Width;
	header.Height = sprite.Height;
	header.FileSize = sizeof(header) + PixelSize + ColorSize;
	header.PixelOffset = sizeof(header);
	header.ColorOffset = sizeof(header) + PixelSize;

	return header;
}

#endif
