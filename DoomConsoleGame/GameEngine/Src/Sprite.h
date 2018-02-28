/*
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "memory.h"
#include "vec2.h"

#pragma pack(push, 16)
struct SpriteHeader
{
	wchar_t Sentinal[4];
	i32		Width;
	i32		Height;
	i32		FileSize;
	i32		PixelOffset;
	i32		ColorOffset;
};

struct Sprite
{
	i32 Width;
	i32 Height;
	wchar_t* Pixels;
	u16 *Colors;
};
#pragma pack(pop)

internal_ Sprite CreateSprite(MemoryHandle memory, i32 W, i32 H)
{
	Sprite sprite = {};
	size_t TotalSize = W * H;
	sprite.Pixels = CreateArray(memory, wchar_t, TotalSize);
	sprite.Colors = CreateArray(memory, u16, TotalSize);

	Assert(sprite.Pixels);
	Assert(sprite.Colors);

	sprite.Width = W;
	sprite.Height = H;

	return sprite;
}

internal_ size_t GetPixelSize(Sprite sprite)
{
	size_t PixelSize = sprite.Width * sprite.Height * sizeof(wchar_t);
	return PixelSize;
}

internal_ size_t GetColorSize(Sprite sprite)
{
	size_t ColorSize = sprite.Width * sprite.Height * sizeof(u16);
	return ColorSize;
}

internal_ SpriteHeader GetSpriteHeader(Sprite sprite)
{
	SpriteHeader header = {};

	size_t PixelSize = GetPixelSize(sprite);
	size_t ColorSize = GetColorSize(sprite);
	
	header.Sentinal[0] = 'S';
	header.Sentinal[1] = 'P';
	header.Sentinal[2] = 'R';
	header.Sentinal[3] = 'T';
	header.Width = sprite.Width;
	header.Height = sprite.Height;
	header.FileSize = sizeof(header) + PixelSize + ColorSize;
	header.PixelOffset = sizeof(header);
	header.ColorOffset = sizeof(header) + PixelSize;

	return header;
}

// Return the color at the specified texel coordinate (0 to 1)
internal_ u16 SampleSprite(vec2 sample, Sprite* img)
{
	u16 Color = 0;

	int sampleX = sample.x * img->Width;
	int sampleY = sample.y * img->Height;

	sampleX = Clamp(0, sampleX, (r32)img->Width - 1);
	sampleY = Clamp(0, sampleY, (r32)img->Height - 1);

	Color = img->Colors[sampleY * img->Width + sampleX];

	return Color;
}

#endif
