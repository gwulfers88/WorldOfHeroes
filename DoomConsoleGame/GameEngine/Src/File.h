/*
	File.h
	George Wulfers
	02/05/2018
	*************************
	Set of functions used for reading and writing data in and out of files.
	It keeps track of FILE pointers that are stored with in an array.
	and a file counter to track how many files are opened at one time.

	The user gets a handle to a file that will be used to index into this array to 
	find the proper FILE that they opened for other operations such as read, write, 
	and fetching the full size of the file.
*/

#ifndef FILE_H
#define FILE_H

#include "platform.h"

#define MaxFileHandles 2
FILE* FileHandles[MaxFileHandles];			// List of opened files that are in memory
global_variable i32 fileHandleCount = 0;	// counter of files

struct FileReadData
{
	void* Data;
	i32 fileSize;
};

i32 platform_getFreeSlot()
{
	i32 Handle = -1;
	for (int i = 0; i < MaxFileHandles; ++i)
	{
		if (FileHandles[i] == 0)
		{
			Handle = i;
			break;
		}
	}

	return Handle;
}

i32 platform_fileOpen(i8* filename, char* mode)
{
	i32 Handle = platform_getFreeSlot();
	Assert((Handle != -1));

	FILE* file = fopen(filename, mode);
	if (file)
	{
		FileHandles[Handle] = file;
		fileHandleCount++;
	}
	else
	{
		Handle = -1;
	}

	return Handle;
}

void platform_fileSeek(i32 Handle, i32 offset)
{
	Assert((Handle < MaxFileHandles));
	FILE* file = FileHandles[Handle];
	if(file)
		fseek(file, offset, SEEK_SET);
}

i32 platform_getFileSize(i32 Handle)
{
	Assert((Handle < MaxFileHandles));
	i32 size = 0;
	FILE* file = FileHandles[Handle];
	if (file)
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);
	}
	else
		size = -1;

	return size;
}

void* platform_fileRead(i32 Handle, i32 sizeToRead)
{
	void* buffer = 0;
	Assert((Handle < MaxFileHandles));
	
	FILE *file = FileHandles[Handle];
	if (file)
	{
		buffer = malloc(sizeToRead);
		fread(buffer, sizeToRead, 1, file);
	}

	return buffer;
}

FileReadData platform_fileReadEntire(i32 Handle)
{
	FileReadData data = {};
	Assert((Handle < MaxFileHandles));
	FILE* file = FileHandles[Handle];
	if (file)
	{
		i32 size = platform_getFileSize(Handle);
		data.Data = platform_fileRead(Handle, size);
		data.fileSize = size;
	}
	return data;
}

void platform_fileWrite(i32 Handle, void* data, i32 sizeToWrite)
{
	Assert((Handle < MaxFileHandles));
	FILE* file = FileHandles[Handle];
	if (file)
	{
		fwrite(data, sizeToWrite, 1, file);
	}
}

void platform_fileClose(i32 Handle)
{
	Assert((Handle < MaxFileHandles));
	FILE* file = FileHandles[Handle];
	if (file)
	{
		fclose(file);
		file = 0;
		FileHandles[Handle] = file;
		fileHandleCount--;
	}
}

#endif