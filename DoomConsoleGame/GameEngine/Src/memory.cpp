#include "memory.h"
#include <memory>

struct memory
{
	void* base;
	size_t size;
	size_t used = 0;
};

struct memory_chunk
{
	void* base;
	size_t size;
	size_t used;
};

#pragma pack(push, 4)
struct memory_header
{
	char id[4];
	size_t offset;
};
#pragma pack(pop)

#define MAX_MEMORY_CHUNKS 2
static memory Memory = {};
static memory_chunk memoryChunks[MAX_MEMORY_CHUNKS] = {};
static int memoryChunksCount = 0;

memory AllocateMemory(size_t size)
{
	memory Result = {};
	Result.base = malloc(size);
	Result.size = size;

	return Result;
}

void FreeMemory(memory* Memory)
{
	if (Memory)
	{
		if (Memory->base)
		{
			free(Memory->base);
			Memory->size = 0;
		}
	}
}

void InitializeMemory(size_t size)
{
	Memory = AllocateMemory(size);
}

void FreeMemory()
{
	FreeMemory(&Memory);
}

MemoryHandle InitializeChunk(size_t size)
{
	MemoryHandle handle = 0;

	if (memoryChunksCount < MAX_MEMORY_CHUNKS)
	{
		memory_chunk *Chunk = memoryChunks + memoryChunksCount;

		if (Memory.used + size <= Memory.size)
		{
			if (Memory.base)
			{
				Chunk->base = (char*)Memory.base + Memory.used;
				Chunk->size = size;
				Chunk->used = 0;

				Memory.used += size;

				memset(Chunk->base, 0, Chunk->size);

				handle = (MemoryHandle)Chunk;
				memoryChunksCount++;
			}
		}
	}

	return handle;
}

void* PushObject(MemoryHandle handle, size_t AllocSize)
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

bool PullObject(MemoryHandle handle, size_t DallocSize)
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
