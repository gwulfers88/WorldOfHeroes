#include <memory>
#include "memory.h"
#include "platform.h"
//
//struct memory
//{
//	void* base;
//	size_t size;
//	size_t used;
//};

#pragma pack(push, 4)
struct memory_header
{
	char id[4];
	size_t offset;
};
#pragma pack(pop)
//
//#define MAX_MEMORY_CHUNKS 2
//static memory Memory = {};
//static memory_chunk memoryChunks[MAX_MEMORY_CHUNKS] = {};
//static int memoryChunksCount = 0;

void* Memory::base = 0;
size_t Memory::size = 0;
size_t Memory::used = 0;

int Memory::handleCount = 0;
Memory::memory_chunk Memory::chunks[MAX_MEMORY_CHUNKS];

void Memory::AllocateMemory(size_t newSize)
{
	base = malloc(newSize);
	size = newSize;
	used = 0;
	Assert(base);
}
//
//void Memory::FreeMemory(memory* Memory)
//{
//	if (Memory)
//	{
//		if (Memory->base)
//		{
//			free(Memory->base);
//			Memory->size = 0;
//		}
//	}
//}

void Memory::InitializeMemory(size_t size)
{
	AllocateMemory(size);
}

void Memory::FreeMemory()
{
	Assert(base);
	free(base);
	base = 0;
	size = 0;
}

MemoryHandle Memory::InitializeChunk(size_t size)
{
	MemoryHandle handle = 0;
	if (handleCount < MAX_MEMORY_CHUNKS)
	{
		memory_chunk* Chunk = chunks + handleCount;
		Assert(Chunk);

		if (used + size <= Memory::size)
		{
			if (base)
			{
				Chunk->base = (char*)base + used;
				Chunk->size = size;
				Chunk->used = 0;

				used += size;

				memset(Chunk->base, 0, Chunk->size);

				handle = (MemoryHandle)Chunk;
				handleCount++;
			}
		}
	}

	return handle;
}

void* Memory::PushObject(MemoryHandle handle, size_t AllocSize)
{
	void* result = 0;
	memory_chunk* Chunk = (memory_chunk*)handle;
	if (Chunk)
	{
		if ((Chunk->used + sizeof(memory_header) + AllocSize) <= Chunk->size)
		{
			if (Chunk->base)
			{
				result = ((char*)Chunk->base + Chunk->used);
				Chunk->used += AllocSize;

				memory_header *header = (memory_header*)((char*)Chunk->base + Chunk->used);
				header->id[0] = 'M';
				header->id[1] = 'E';
				header->id[2] = 'M';
				header->id[3] = 'H';
				header->offset = AllocSize;

				Chunk->used += sizeof(memory_header);
			}
		}
	}
	return result;
}

bool Memory::PullObject(MemoryHandle handle, size_t DallocSize)
{
	bool result = false;
	memory_chunk* Chunk = (memory_chunk*)handle;
	if (Chunk)
	{
		if ((int)(Chunk->used - DallocSize - sizeof(memory_header)) >= 0)
		{
			if (Chunk->base)
			{
				memory_header* header = (memory_header*)((char*)Chunk->base + Chunk->used - sizeof(memory_header));
				if (header)
				{
					if (header->offset == DallocSize)
					{
						Chunk->used -= DallocSize + sizeof(memory_header);
						memset(((char*)Chunk->base + Chunk->used), 0, Chunk->size - Chunk->used);
						result = true;
					}
				}
			}
		}
	}
	return result;
}
