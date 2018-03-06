/*
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "memory.h"
#include "File.h"
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

struct SpriteSample
{
	u16		Color;
	wchar_t Pixel;
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
internal_ SpriteSample SampleSprite(vec2 sample, Sprite* img)
{
	SpriteSample Result = {};

	int sampleX = sample.x * img->Width;
	int sampleY = sample.y * img->Height;

	sampleX = Clamp(0, sampleX, (r32)img->Width - 1);
	sampleY = Clamp(0, sampleY, (r32)img->Height - 1);

	Result.Color = img->Colors[sampleY * img->Width + sampleX];
	Result.Pixel = img->Pixels[sampleY * img->Width + sampleX];

	return Result;
}

internal_ void LoadSprite(char* filename, Sprite* img)
{
	// Load the pillar texture
	int spriteHandle = platform_fileOpen(filename, "rb");
	FileReadData fileData = platform_fileReadEntire(spriteHandle);
	platform_fileClose(spriteHandle);
	SpriteHeader* header = (SpriteHeader*)fileData.Data;
	if (header)
	{
		if (header->Sentinal[0] == 'S' && header->Sentinal[1] == 'P' && header->Sentinal[2] == 'R' && header->Sentinal[3] == 'T')
		{
			img->Width = header->Width;
			img->Height = header->Height;
			u16* colors = (u16*)((u8*)fileData.Data + header->ColorOffset);
			wchar_t* pixels = (wchar_t*)((u8*)fileData.Data + header->PixelOffset);
			img->Colors = CreateArray(Memory::GetPersistantHandle(), u16, img->Width * img->Height);
			img->Pixels = CreateArray(Memory::GetPersistantHandle(), wchar_t, img->Width * img->Height);

			memcpy_s(img->Colors, img->Width*img->Height * sizeof(u16), colors, img->Width*img->Height * sizeof(u16));
			memcpy_s(img->Pixels, img->Width*img->Height * sizeof(wchar_t), pixels, img->Width*img->Height * sizeof(wchar_t));

			if (fileData.Data)
				free(fileData.Data);
		}
	}
}

#endif
