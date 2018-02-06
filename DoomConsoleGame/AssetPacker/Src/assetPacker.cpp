
#pragma comment(lib, "GameEngine_Debug.lib")

#include "platform.h"
#include "Sprite.h"

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

#include <iostream>

char LoadPath[] = "Save/";
char FileExt[] = ".sprt";

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

#define READ_ENTIRE_FILE(name) FileRead name(MemoryHandle memoryHandle, char* filename)
typedef READ_ENTIRE_FILE(read_entire_file);
READ_ENTIRE_FILE(ReadEntireFile)
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
	size_t totalSize = strlen(LoadPath) + strlen(filename) + strlen(FileExt);
	char* buffer = CreateArray(memoryHandle, char, totalSize);
	strcat(buffer, LoadPath);
	strcat(buffer, filename);
	strcat(buffer, FileExt);

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
		MessageBox(0, "Incorrect file format.", "ERROR:", MB_ICONERROR | MB_OK);
	}

	PullArray(memoryHandle, char, totalSize);	// Release the string buffer we allocated earlier

	return sprite;
}

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
	FileRead *fileReads = CreateArray(memory, FileRead, numberOfFiles);

	FILE* saveFile = fopen(Packs.PackName, "wb");
	Assert(saveFile);
	fwrite(&PackHeader, sizeof(pack_header), 1, saveFile);

	printf("Saving pack file directories\n");
	for (int i = 0; i < numberOfFiles; i++)
	{
		char name[56] = "Save/";
		strcat(name, filenames[i]);
		fileReads[i] = ReadEntireFile(memory, name);
		
		char pakname[56] = "img/";
		strcat(pakname, filenames[i]);
		strcpy(packFiles[i].Filename, pakname);
		packFiles[i].FilePosition = packFilePosition;
		packFiles[i].FileLength = (fileReads[i].fileSize - sizeof(SpriteHeader) + (sizeof(int) * 2));

		packFilePosition += sizeof(packFiles[i]);

		fwrite(&packFiles[i], sizeof(pack_file), 1, saveFile);
	}

	Packs.PackFiles = packFiles;

	printf("Saving image data\n");
	for (int i = 0; i < numberOfFiles; i++)
	{
		FileRead file = fileReads[i];
		SpriteHeader* spriteHeader = (SpriteHeader*)file.Data;

		Sprite *img = sprites + i;
		img->Width = spriteHeader->Width;
		img->Height = spriteHeader->Height;
		img->Pixels = (wchar_t*)((i8*)file.Data + spriteHeader->PixelOffset);
		img->Colors = (u16*)((i8*)file.Data + spriteHeader->ColorOffset);

		fwrite(img, sizeof(Sprite), 1, saveFile);
	}

	printf("Saving complete\n");
	fclose(saveFile);


	printf("size of long: %d\n\n", sizeof(long));

	system("pause");

	Memory::FreeMemory();

	return 0;
}