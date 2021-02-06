#pragma once

#include "MemManager.h"



MemManager* MemManager::create( void* startPtr, size_t size, unsigned int n)
{
	MemManager* HeapMan = new MemManager();
	HeapMan->startPointer = startPtr;
	HeapMan->memorySize = size;
	HeapMan->memoryUsed = 0;
	HeapMan->nDescriptors = n;
	HeapMan->endPointer = (char*)HeapMan->startPointer + HeapMan->memorySize - (HeapMan->nDescriptors * sizeof(Descriptor) * 2);
	HeapMan->usedDesc = (Descriptor*)((char*)HeapMan->endPointer);
	HeapMan->nUsedDesc = 0;
	HeapMan->freeDesc = (Descriptor*)((char*)HeapMan->endPointer + sizeof(Descriptor)*HeapMan->nDescriptors);
	HeapMan->nFreeDesc = 1;
	HeapMan->freeDesc[0].offset = HeapMan->startPointer;
	HeapMan->freeDesc[0].size = (int)HeapMan->endPointer - (int)HeapMan->startPointer;
	return HeapMan;

}


void * MemManager::alloc(size_t size, unsigned int alignment) {
	return startPointer;
}

void * MemManager::alloc(size_t size) {
	size_t i_size = (((size - 1) / 4) + 1) * 4;
	for (unsigned int i = 0; i < nFreeDesc; i++) {


		if (freeDesc[i].size < i_size) {
			for (unsigned int x = 0; x < i_size; x++) {
				char* temp = (char*)freeDesc[i].offset;
				*temp = '|';
			}
			void * ptr = (void*)freeDesc[i].offset;
			usedDesc[nUsedDesc].offset = freeDesc[i].offset;
			usedDesc[nUsedDesc].size = i_size;
			freeDesc[i].offset += i_size;
			freeDesc[i].size -= i_size;
			nUsedDesc++;
			return ptr;

		}

	}
	return nullptr;
}
void MemManager::collect()
{
	if (nFreeDesc > 1)
	{
		for (unsigned int i = 0; i < nFreeDesc; i++)
		{
			void* endPoint = freeDesc[i].offset + freeDesc[i].size;
			for (unsigned int k = 1; k < nFreeDesc; k++)
			{


				if (endPoint == freeDesc[k].offset)
				{
					freeDesc[i].size += freeDesc[k].size;
					if (k+ 1 < nFreeDesc)
					{
						for (unsigned int j = k ; j < nFreeDesc-1; i++)
						{
							freeDesc[j].offset = freeDesc[j + 1].offset;
							freeDesc[j].size = freeDesc[j + 1].size;
						}
					}

					nFreeDesc--;
				}
			}
		}
	}
}

void MemManager::free(void * ptr) {
	for (unsigned int i = 0; i < nUsedDesc; i++) {
		if (usedDesc[i].offset == (Descriptor*)ptr)
		{
			nFreeDesc++;
			freeDesc[nFreeDesc].offset = (Descriptor*)ptr;
			freeDesc[nFreeDesc].size = usedDesc[i].size;
			for (unsigned int j = i; j < nUsedDesc-1; j++)
			{
				usedDesc[j].offset = usedDesc[j+1].offset;
				usedDesc[j].size = usedDesc[j+1].size;
			}
			nUsedDesc--;
			
		}
	}
	collect();
}

bool MemManager::IsAllocated(void* ptr)
{
	for (unsigned int i = 0; i < nUsedDesc; i++) {
		if (usedDesc[i].offset == (Descriptor*)ptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

size_t MemManager::getLargestFreeBlock()
{
	size_t temp=freeDesc[0].size;
	for (unsigned int i = 0; i < nFreeDesc; i++)
	{
	
		if (freeDesc[i].size < freeDesc[i + 1].size)
		{
			temp = freeDesc[i+1].size;
		}
		
	}
	return temp;
}
bool  MemManager::Contains(void *ptr)
{
	if (ptr > startPointer && ptr < endPointer)
	{
		return true;
	}
	else
		return false;
}

size_t  MemManager::getTotalFreeMemory()
{
	size_t totalMemory;
	for (unsigned int i = 0; i < nFreeDesc; i++)
	{
		totalMemory += freeDesc[i].size;
	}
	return totalMemory;
}

void MemManager::ShowFreeBlocks()
{

	for (unsigned int i = 0; i < nFreeDesc; i++)
	{
		printf("Free Descriptor %d, offset :%p , size: %d",i+1,freeDesc[i].offset, freeDesc[i].size );
	}
}