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

typedef void* MemoryHandle;
#define MAX_MEMORY_CHUNKS 2

class Memory
{

	struct memory_chunk
	{
		void* base;
		size_t size;
		size_t used;
	};

public:
	~Memory(){}

	static void InitializeMemory(size_t size);
	static void FreeMemory();
	static MemoryHandle InitializeChunk(size_t size);
	static void* PushObject(MemoryHandle handle, size_t AllocSize);
	static bool PullObject(MemoryHandle handle, size_t DallocSize);

private:
	static void AllocateMemory(size_t size);
	//static void FreeMemory(memory* Memory);

	static void* base;
	static size_t size;
	static size_t used;

	static int handleCount;
	static memory_chunk chunks[MAX_MEMORY_CHUNKS];
};

//void InitializeMemory(size_t size);
//void FreeMemory();
//MemoryHandle InitializeChunk(size_t size);
#define CreateVoidPtr(handle, type, Count) (type*)Memory::PushObject(handle, Count)
#define CreateObject(handle, type) new((type*)Memory::PushObject(handle, sizeof(type))) type
#define CreateArray(handle, type, Count) (type*)Memory::PushObject(handle, sizeof(type)*Count)
//void* PushObject(MemoryHandle Chunk, size_t AllocSize);
#define PullVoidPtr(handle, type, Count) Memory::PullObject(handle, Count)
#define PullStruct(handle, type) Memory::PullObject(handle, sizeof(type))
#define PullArray(handle, type, Count) Memory::PullObject(handle, sizeof(type)*Count)
//bool PullObject(MemoryHandle Chunk, size_t DallocSize);

#endif