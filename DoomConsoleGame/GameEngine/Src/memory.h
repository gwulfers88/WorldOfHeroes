/*******************************************************
FileName: memory.h
StartDate: 01/18/2018
ModDate:
CreatorName: George Wulfers
ContributorsName:
===================================================
most of the memory management processes will be here.
*******************************************************/

// TOOD: Try to hard cast a child class 
// TODO: Look into refptr

#ifndef MEMORY_H
#define MEMORY_H

typedef void* MemoryHandle;

void InitializeMemory(size_t size);
void FreeMemory();
MemoryHandle InitializeChunk(size_t size);
#define CreateObject(handle, type) (type*)PushObject(handle, sizeof(type))
#define CreateArray(handle, type, Count) (type*)PushObject(handle, sizeof(type)*Count)
void* PushObject(MemoryHandle Chunk, size_t AllocSize);
#define PullStruct(handle, type) PullObject(handle, sizeof(type))
#define PullArray(handle, type, Count) PullObject(handle, sizeof(type)*Count)
bool PullObject(MemoryHandle Chunk, size_t DallocSize);

#endif