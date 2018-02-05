/*******************************************************
FileName: memory.h
StartDate: 01/18/2018
ModDate:
CreatorName: George Wulfers
ContributorsName:
===================================================
most of the memory management processes will be here.
*******************************************************/

// TODO: Look into refptr

#ifndef MEMORY_H
#define MEMORY_H

#include "platform.h"

typedef void* MemoryHandle;
#define MAX_MEMORY_CHUNKS 2

class Memory
{
	struct memory_chunk
	{
		void* base;
		mem_size size;
		mem_size used;
	};

public:
	~Memory(){}

	// Allocates base meory
	static void InitializeMemory(mem_size size);

	// Free base memory
	static void FreeMemory();

	// Partitions main memory.
	static MemoryHandle InitializeChunk(mem_size size);

	// allocates memory from partition
	static void* PushObject(MemoryHandle handle, mem_size AllocSize);

	// releases memory from partition
	static bool PullObject(MemoryHandle handle, mem_size DallocSize);

private:
	static void* base;
	static mem_size size;
	static mem_size used;

	static int handleCount;
	static memory_chunk chunks[MAX_MEMORY_CHUNKS];
};

#define CreateVoidPtr(handle, type, Count) (type*)Memory::PushObject(handle, Count)
#define CreateObject(handle, type) new((type*)Memory::PushObject(handle, sizeof(type))) type
#define CreateArray(handle, type, Count) (type*)Memory::PushObject(handle, sizeof(type)*Count)
#define PullVoidPtr(handle, type, Count) Memory::PullObject(handle, Count)
#define PullStruct(handle, type) Memory::PullObject(handle, sizeof(type))
#define PullArray(handle, type, Count) Memory::PullObject(handle, sizeof(type)*Count)

#endif