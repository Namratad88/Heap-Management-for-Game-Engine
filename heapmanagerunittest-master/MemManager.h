#pragma once

#include <cassert>
#include <iostream>

struct Descriptor{
		void* offset;
		unsigned int size;
};

class MemManager {
public:
	static MemManager* create( void* startPtr, size_t size, unsigned int n);
	void * alloc(size_t size);
	void * alloc(size_t size, unsigned int alignment);
	void  free(void* ptr);
	bool Contains(void *i_ptr);
	bool IsAllocated(void* ptr);
	size_t getLargestFreeBlock();
	size_t getTotalFreeMemory();
	void ShowFreeBlocks();
	void* startPointer;
	void* endPointer;
	void collect();
	size_t memorySize = 0;
	size_t _memoryAlloc = 0;
	size_t memoryUsed = 0;
	unsigned int nDescriptors = 0;
	unsigned int nUsedDesc = 0;
	unsigned int nFreeDesc = 0;
	Descriptor * usedDesc;
	Descriptor * freeDesc;





};


