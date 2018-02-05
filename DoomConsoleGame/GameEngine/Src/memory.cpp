#include <memory>
#include "memory.h"

#pragma pack(push, 4)
struct memory_header
{
	i8 id[4];
	size_t offset;
};
#pragma pack(pop)

void* Memory::base = 0;
mem_size Memory::size = 0;
mem_size Memory::used = 0;

i32 Memory::handleCount = 0;
Memory::memory_chunk Memory::chunks[MAX_MEMORY_CHUNKS];

void Memory::InitializeMemory(mem_size newSize)
{
	base = malloc(newSize);
	size = newSize;
	used = 0;
	Assert(base);
}

void Memory::FreeMemory()
{
	Assert(base);
	free(base);
	base = 0;
	size = 0;
}

MemoryHandle Memory::InitializeChunk(mem_size size)
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
				Chunk->base = (i8*)base + used;
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

void* Memory::PushObject(MemoryHandle handle, mem_size AllocSize)
{
	void* result = 0;
	memory_chunk* Chunk = (memory_chunk*)handle;
	if (Chunk)
	{
		if ((Chunk->used + sizeof(memory_header) + AllocSize) <= Chunk->size)
		{
			if (Chunk->base)
			{
				result = ((i8*)Chunk->base + Chunk->used);
				Chunk->used += AllocSize;

				memory_header *header = (memory_header*)((i8*)Chunk->base + Chunk->used);
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

bool Memory::PullObject(MemoryHandle handle, mem_size DallocSize)
{
	bool result = false;
	memory_chunk* Chunk = (memory_chunk*)handle;
	if (Chunk)
	{
		if ((i32)(Chunk->used - DallocSize - sizeof(memory_header)) >= 0)
		{
			if (Chunk->base)
			{
				memory_header* header = (memory_header*)((i8*)Chunk->base + Chunk->used - sizeof(memory_header));
				if (header)
				{
					if (header->offset == DallocSize)
					{
						Chunk->used -= DallocSize + sizeof(memory_header);
						memset(((i8*)Chunk->base + Chunk->used), 0, Chunk->size - Chunk->used);
						result = true;
					}
				}
			}
		}
	}
	return result;
}
