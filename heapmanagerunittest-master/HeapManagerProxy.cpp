
#include "HeapManagerProxy.h"
#include <assert.h>
#include <stdio.h>





namespace HeapManagerProxy
{

	HeapManager * CreateHeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors)
	{

		HeapManager* HeapMan = new HeapManager();
		HeapMan->startPointer = i_pMemory;
		HeapMan->memorySize = i_sizeMemory;
		HeapMan->memoryUsed = 0;
		HeapMan->nDescriptors = i_numDescriptors;
		HeapMan->endPointer = (char*)HeapMan->startPointer + HeapMan->memorySize - (HeapMan->nDescriptors * sizeof(Descriptor) * 2);
		HeapMan->usedDesc = (Descriptor*)((char*)HeapMan->endPointer);
		HeapMan->nUsedDesc = 0;
		HeapMan->freeDesc = (Descriptor*)((char*)HeapMan->endPointer + sizeof(Descriptor)*HeapMan->nDescriptors);
		HeapMan->nFreeDesc = 1;
		HeapMan->freeDesc[0].offset = HeapMan->startPointer;
		HeapMan->freeDesc[0].size = (int)HeapMan->endPointer - (int)HeapMan->startPointer;
		return HeapMan;

	}

	void Destroy(HeapManager * i_pManager)
	{
		assert(i_pManager);

	//	i_pManager->destroy();
	}

	void * alloc(HeapManager * i_pManager, size_t i_size)
	{
		//i_size = (((i_size - 1) / 4) + 1) * 4;
		void * off = (char*)i_pManager->endPointer - i_size;
		
		for (unsigned int i = 0; i < i_pManager->nFreeDesc; i++) {


			if (i_pManager->freeDesc[i].size > i_size) {

				void * ptr = (void*)((uintptr_t)i_pManager->freeDesc[i].offset + i_pManager->freeDesc[i].size - i_size);
				i_pManager->usedDesc[i_pManager->nUsedDesc].offset = ptr;
				i_pManager->usedDesc[i_pManager->nUsedDesc].size = i_size;
				//->freeDesc[i].offset += i_size;
				i_pManager->freeDesc[i].size -= i_size;
				i_pManager->nUsedDesc++;
				return i_pManager->usedDesc[i_pManager->nUsedDesc].offset;
			}
			//else if (i_pManager->freeDesc[i].size == i_size) {
			//	void * ptr = (void*)((uintptr_t)i_pManager->freeDesc[i].offset + i_pManager->freeDesc[i].size - i_size);
			//	i_pManager->usedDesc[i_pManager->nUsedDesc].offset = ptr;
			//	i_pManager->usedDesc[i_pManager->nUsedDesc].size = i_size;
			//	//->freeDesc[i].offset += i_size;
			//	//i_pManager->freeDesc[i].size -= i_size;
			//	i_pManager->nFreeDesc--;
			//	i_pManager->nUsedDesc++;
			//	return ptr;
			//}
			
		}
		return nullptr;
	}

	void * alloc(HeapManager * i_pManager, size_t i_size, unsigned int i_alignment)
	{
		return nullptr;
	}

	bool free(HeapManager * i_pManager, void * i_ptr)
	{
		for (unsigned int i = 0; i < i_pManager->nUsedDesc; i++) {
			if (i_pManager->usedDesc[i].offset == (Descriptor*)i_ptr)
			{
				i_pManager->nFreeDesc++;
				i_pManager->freeDesc[i_pManager->nFreeDesc].offset = (Descriptor*)i_ptr;
				i_pManager->freeDesc[i_pManager->nFreeDesc].size = i_pManager->usedDesc[i].size;
				for (unsigned int j = i; j < i_pManager->nUsedDesc - 1; j++)
				{
					i_pManager->usedDesc[j].offset = i_pManager->usedDesc[j + 1].offset;
					i_pManager->usedDesc[j].size = i_pManager->usedDesc[j + 1].size;
				}
				i_pManager->nUsedDesc--;
				Collect(i_pManager);
				return true;
			}
		}
		
		return false; 

	}

	void Collect(HeapManager * i_pManager)
	{
		if (i_pManager->nFreeDesc > 1)
		{
			for (unsigned int i = 0; i < i_pManager->nFreeDesc; i++)
			{
				void* endPoint = (void*)((uintptr_t)i_pManager->freeDesc[i].offset + i_pManager->freeDesc[i].size);
				for (unsigned int k = 1; k < i_pManager->nFreeDesc; k++)
				{


					if (endPoint == i_pManager->freeDesc[k].offset)
					{
						i_pManager->freeDesc[i].size += i_pManager->freeDesc[k].size;
						if (k + 1 < i_pManager->nFreeDesc)
						{
							for (unsigned int j = k; j < i_pManager->nFreeDesc - 1; i++)
							{
								i_pManager->freeDesc[j].offset = i_pManager->freeDesc[j + 1].offset;
								i_pManager->freeDesc[j].size = i_pManager->freeDesc[j + 1].size;
							}
						}

						i_pManager->nFreeDesc--;
					}
				}
			}
		}
	}


	bool Contains( const HeapManager * i_pManager, void * i_ptr)
	{
		if (i_ptr >= i_pManager->startPointer && i_ptr <= i_pManager->endPointer)
		{
			return true;
		}
		else
			return false;
	}

	bool IsAllocated(const HeapManager * i_pManager, void * i_ptr)
	{
		for (unsigned int i = 0; i < i_pManager->nUsedDesc; i++) {
			printf("\nTest => %d %d\n", (uintptr_t)i_pManager->usedDesc[i].offset, (uintptr_t)i_ptr);
			if ((uintptr_t)i_pManager->usedDesc[i].offset == (uintptr_t)i_ptr)
			{
				return true;
			}
			else
			{
				return false;
			}
			
		}
	}

	size_t GetLargestFreeBlock(const HeapManager * i_pManager)
	{
		size_t temp = i_pManager->freeDesc[0].size;
		for (unsigned int i = 0; i < i_pManager->nFreeDesc; i++)
		{

			if (i_pManager->freeDesc[i].size < i_pManager->freeDesc[i + 1].size)
			{
				temp = i_pManager->freeDesc[i + 1].size;
			}

		}
		return temp;
	}

	size_t GetTotalFreeMemory(const HeapManager * i_pManager)
	{
		size_t totalMemory;
		for (unsigned int i = 0; i < i_pManager->nFreeDesc; i++)
		{
			totalMemory += i_pManager->freeDesc[i].size;
		}
		return totalMemory;
	}

	void ShowFreeBlocks(const HeapManager * i_pManager)
	{
		for (unsigned int i = 0; i < i_pManager->nFreeDesc; i++)
		{
			printf("Free Descriptor %d, offset :%p , size: %d", i + 1, i_pManager->freeDesc[i].offset, i_pManager->freeDesc[i].size);
		}
	}

	void ShowOutstandingAllocations(const HeapManager * i_pManager)
	{
		//assert(i_pManager);

#ifdef __TRACK_ALLOCATIONS
		i_pManager->ShowOutstandingAllocations();
#else
		printf("HeapManager compiled without __TRACK_ALLOCATIONS defined.\n");
#endif

	}

} // namespace HeapManagerProxy; 