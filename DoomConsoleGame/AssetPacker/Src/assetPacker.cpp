
#pragma comment(lib, "GameEngine_Debug.lib")

#include "platform.h"
#include "File.h"
#include "Sprite.h"

#include <iostream>

#pragma pack(push, 16)
struct pack_header
{
	i8	Id[4];		// id to make sure pak header is valid "PACK"
	i32 DirectoryPosition;		// Index to the beginning of the file table
	i32 DirectoryLength;	// Size of the entire file table
};

struct pack_file
{
	i8	Filename[56];	// name of the file including folders. "path/file.ext"
	i32 FilePosition;			// Offset to the start of the file contents starting from the begging of the pak file
	i32 FileLength;		// size of this file.
};

struct pack
{
	i8  PackName[128];
	i32 PackHandle;
	i32 NumberOfFiles;
	pack_file* PackFiles;
};
#pragma pack(pop)

char LoadPath[] = "Save/";
char FileExt[] = ".sprt";

int main()
{
	int numberOfFiles = 0;

	Memory::InitializeMemory(Megabytes(10));
	MemoryHandle memory = Memory::InitializeChunk(Megabytes(10));

	printf("How many assets are you adding? ");
	std::cin >> numberOfFiles;
	Assert(numberOfFiles > 0);

	char *filenames[256];
	u32 filenameSize = 56;
	for (int i = 0; i < numberOfFiles; ++i)
	{
		printf("filename %d: ", i);
		filenames[i] = CreateArray(memory, char, filenameSize);
		char name[56];
		std::cin >> name;
		
		strcat(filenames[i], name);
		strcat(filenames[i], ".sprt");
	}

	for (int i = 0; i < numberOfFiles; ++i)
	{
		printf("filename %s\n", filenames[i]);
	}

	printf("Preparing to save\n");
	pack_header PackHeader = {};
	PackHeader.Id[0] = 'P'; PackHeader.Id[1] = 'A'; PackHeader.Id[2] = 'C'; PackHeader.Id[3] = 'K';
	PackHeader.DirectoryLength = numberOfFiles * sizeof(pack_file);
	PackHeader.DirectoryPosition = sizeof(PackHeader);

	pack Packs = {};

	Packs.NumberOfFiles = numberOfFiles;
	Packs.PackHandle = 0;
	strcpy(Packs.PackName, "Save/pack01.pak");

	i32 packFilePosition = PackHeader.DirectoryPosition;
	pack_file *packFiles = CreateArray(memory, pack_file, numberOfFiles);
	Sprite *sprites = CreateArray(memory, Sprite, numberOfFiles);
	FileReadData *fileReads = CreateArray(memory, FileReadData, numberOfFiles);

	i32 packHanlde = platform_fileOpen(Packs.PackName, "wb");
	platform_fileWrite(packHanlde, &PackHeader, sizeof(PackHeader));
	
	printf("Saving pack file directories\n");
	for (int i = 0; i < numberOfFiles; i++)
	{
		char name[56] = "Save/";
		strcat(name, filenames[i]);
		i32 fileHandle = platform_fileOpen(name, "rb");
		fileReads[i] = platform_fileReadEntire(fileHandle);
		platform_fileClose(fileHandle);

		char pakname[56] = "img/";
		strcat(pakname, filenames[i]);
		strcpy(packFiles[i].Filename, pakname);
		packFiles[i].FilePosition = packFilePosition;
		packFiles[i].FileLength = (fileReads[i].fileSize - sizeof(SpriteHeader) + (sizeof(int) * 2));

		packFilePosition += sizeof(packFiles[i]);

		platform_fileWrite(packHanlde, &packFiles[i], sizeof(packFiles[i]));
	}

	Packs.PackFiles = packFiles;

	printf("Saving image data\n");
	for (int i = 0; i < numberOfFiles; i++)
	{
		FileReadData file = fileReads[i];
		SpriteHeader* spriteHeader = (SpriteHeader*)file.Data;

		Sprite *img = sprites + i;
		img->Width = spriteHeader->Width;
		img->Height = spriteHeader->Height;
		img->Pixels = (wchar_t*)((i8*)file.Data + spriteHeader->PixelOffset);
		img->Colors = (u16*)((i8*)file.Data + spriteHeader->ColorOffset);

		platform_fileWrite(packHanlde, img, sizeof(*img));
	}

	printf("Saving complete\n");
	platform_fileClose(packHanlde);

	printf("size of long: %d\n\n", sizeof(long));

	system("pause");

	for (int i = 0; i < ArrayCount(fileReads); i++)
	{
		free(fileReads[i].Data);
		fileReads[i].Data = 0;
		fileReads[i].fileSize = 0;
	}

	Memory::FreeMemory();

	return 0;
}