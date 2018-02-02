#include "SpriteEditor.h"
#include "Sprite.h"

Sprite sprite;
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

void WriteFile(MemoryHandle memoryHandle, Sprite sprite, char* filename)
{
	size_t totalSize = strlen(Path) + strlen(filename) + strlen(fileExt);
	char* buffer = CreateArray(memoryHandle, char, totalSize);
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

	PullArray(memoryHandle, char, totalSize);
}

FileRead ReadEntireFile(MemoryHandle memoryHandle, char* filename)
{
	FileRead fileRead = {};

	FILE* file = fopen(filename, "rb");
	long fileSize = GetFileSize(file);
	fileRead.fileSize = fileSize;
	fileRead.Data = CreateVoidPtr(memoryHandle, void, fileSize);

	Assert(fileRead.Data);
	fread(fileRead.Data, fileSize, 1, file);
	fclose(file);

	return fileRead;
}

Sprite LoadSprite(MemoryHandle memoryHandle, char* filename)
{
	size_t totalSize = strlen(Path) + strlen(filename) + strlen(fileExt);
	char* buffer = CreateArray(memoryHandle, char, totalSize);
	strcat(buffer, Path);
	strcat(buffer, filename);
	strcat(buffer, fileExt);

	Sprite sprite = {};
	FileRead fileRead = ReadEntireFile(memoryHandle, buffer);	// Read the entire file into a buffer

	SpriteHeader* header = (SpriteHeader*)fileRead.Data;	// Fetch the header file
	Assert(header);	// make sure that we actually got a header 

					// Make sure that the file identifier is set properly other wise this is not a 
					// SPRT image
	if (header->Sentinal[0] == 'G' && header->Sentinal[1] == 'W')
	{
		Assert(header->FileSize == fileRead.fileSize);	// Make sure that the size of the file matches the headers file size
		sprite = CreateSprite(memoryHandle, header->Width, header->Height);	// Create sprite object
		size_t PixelSize = GetPixelSize(sprite);	//Calculate sizes for Pixel array and Color array
		size_t ColorSize = GetColorSize(sprite);

		sprite.Pixels = (wchar_t*)((char*)fileRead.Data + header->PixelOffset);			// Fetching Pixel data
		sprite.Colors = (unsigned short*)((char*)fileRead.Data + header->ColorOffset);	// Fetching Color data
	}
	else	// This file is not in our SPRT format
	{
		MessageBox(0, L"Incorrect file format.", L"ERROR:", MB_ICONERROR | MB_OK);
	}

	PullArray(memoryHandle, char, totalSize);	// Release the string buffer we allocated earlier

	return sprite;
}

void SpriteEditor::LoadContent()
{
	int screenWidth = renderer.GetRenderBuffers()->Width;
	int screenHeight = renderer.GetRenderBuffers()->Height;

	if (ArrayCount(loadFilename) > 4)
		sprite = LoadSprite(persistantHandle, loadFilename);
	else
		sprite = CreateSprite(persistantHandle, screenWidth / 2, screenHeight / 2);

	CanvasX = (int)((screenWidth / 2.0f) - (sprite.Width / 2.0f));
	CanvasY = (int)((screenHeight / 2.0f) - (sprite.Height / 2.0f));

	CursorX = 0;
	CursorY = 0;
	CursorSpeed = 5;
	CursorRadius = 2.0f;

	unsigned short colors[] =
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
			PIXEL_COLOR_WHITE
	};

	Colors = colors;
	MaxColors = ArrayCount(Colors);
	colorID = (int)ArrayCount(Colors) - 1;

	ColorsX = (int)((screenWidth / 2.0f) - ((colorID * 2) / 2.0f));;
	ColorsY = 5;
}

bool SpriteEditor::Update(float deltaTime)
{
	bool Quit = false;
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
	if (colorID >= MaxColors)
	{
		colorID = MaxColors - 1;
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

	renderer.ClearBuffer(PIXEL_COLOR_GREY);

	// Draw Color pallete
	for (int i = 0; i < MaxColors; ++i)
	{
		if ((int)colorID == i)
		{
			renderer.DrawPixel(ColorsX + (i * 2) + 1, ColorsY, PIXEL_SOLID, PIXEL_COLOR_DARK_CYAN);
			renderer.DrawPixel(ColorsX + (i * 2) - 1, ColorsY, PIXEL_SOLID, PIXEL_COLOR_DARK_CYAN);
			renderer.DrawPixel(ColorsX + (i * 2), ColorsY + 1, PIXEL_SOLID, PIXEL_COLOR_DARK_CYAN);
			renderer.DrawPixel(ColorsX + (i * 2), ColorsY - 1, PIXEL_SOLID, PIXEL_COLOR_DARK_CYAN);
		}

		renderer.DrawPixel(ColorsX + (i * 2), ColorsY, PIXEL_SOLID, Colors[i]);
	}

	// Draw Canvas
	for (int y = 0; y < sprite.Height + 1; ++y)
	{
		for (int x = 0; x < sprite.Width + 1; ++x)
		{
			// Top and Bottom Lines
			if (y == 0)
				renderer.DrawPixel(x + (CanvasX - 1), y + (CanvasY - 1), PIXEL_SOLID, PIXEL_COLOR_WHITE);
			if (y == sprite.Height)
				renderer.DrawPixel(x + (CanvasX), y + (CanvasY), PIXEL_SOLID, PIXEL_COLOR_WHITE);

			if (x == 0)
				renderer.DrawPixel(x + (CanvasX - 1), y + (CanvasY - 1), PIXEL_SOLID, PIXEL_COLOR_WHITE);
			if (x == sprite.Width)
				renderer.DrawPixel(x + (CanvasX), y + (CanvasY), PIXEL_SOLID, PIXEL_COLOR_WHITE);
		}
	}

	renderer.DrawSprite(CanvasX, CanvasY, sprite.Width, sprite.Height, sprite.Pixels, sprite.Colors);

	// Draw Cursor
	for (int y = 0; y < CursorRadius; ++y)
	{
		for (int x = 0; x < CursorRadius; ++x)
		{
			renderer.DrawPixel(CanvasX + (int)CursorX + (int)(x), CanvasY + (int)CursorY + (int)(y), PIXEL_SEMI_DARK, PIXEL_COLOR_LIGHT_CYAN);
		}
	}

	renderer.PresentBuffer();

	return Quit;
}

void SpriteEditor::UnloadContent()
{
	if (ArrayCount(saveFilename) > 4)
		WriteFile(persistantHandle, sprite, saveFilename);
}
